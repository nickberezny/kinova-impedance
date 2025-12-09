/*
* KINOVA (R) KORTEX (TM)
*
* Copyright (c) 2018 Kinova inc. All rights reserved.
*
* This software may be modified and distributed
* under the terms of the BSD 3-Clause license.
*
* Refer to the LICENSE file for details.
*
*/

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath> 

#include <ctime>
#include <sstream>
#include <fstream> 

#include <KDetailedException.h>

#include <BaseClientRpc.h>
#include <BaseCyclicClientRpc.h>
#include <SessionClientRpc.h>
#include <SessionManager.h>

#include <RouterClient.h>
#include <TransportClientTcp.h>
#include <TransportClientUdp.h>

#include <google/protobuf/util/json_util.h>

#include "kdl/jacobian.hpp"
#include "kdl/tree.hpp"
#include "kdl/chain.hpp"
#include "kdl/frames.hpp"
#include "kdl/jntarray.hpp"
#include "kdl/chainiksolverpos_lma.hpp"
#include <kdl_parser/kdl_parser.hpp>
#include "kdl/chainjnttojacsolver.hpp"
#include "kdl/chaindynparam.hpp"
#include <chainfksolvervel_recursive.hpp>
#include <chainfksolverpos_recursive.hpp>

#include "kinovaDynamics.h"
#include "kinovaDynamics_initialize.h"
#include "kinovaDynamics_terminate.h"
#include "rt_nonfinite.h"

#include <chrono>

#include "utilities.h"
#include "../include/structs.h"
#include "../include/inverse_kinematics.h"
#include "../include/dataLogger.h"
#include "../include/safety.h"
#include "../include/forceSensor.h"
#include "../include/admittance.h"
#include "../include/inverse_dynamics.h"

#if defined(_MSC_VER)
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <time.h>

namespace k_api = Kinova::Api;
using Eigen::MatrixXd;
using Eigen::VectorXd;

#define PORT 10000
#define PORT_REAL_TIME 10001

#define DURATION 10             // Network timeout (seconds)

float velocity = 20.0f;         // Default velocity of the actuator (degrees per seconds)
float time_duration = DURATION; // Duration of the example (seconds)

// Waiting time during actions
const auto ACTION_WAITING_TIME = std::chrono::seconds(1);
const double PI = 3.14159265358979323846;

std::ofstream outputFile; //log file

double q[3][DURATION*1000];
double qd[3][DURATION*1000];
double z[DURATION*1000];
double Fsense[DURATION*1000];
double Fest[DURATION*1000];
double iact[DURATION*1000];

double q_cur[3];
double q_prev[3];
double qd_cur[3];
double qdd_cur[3];
double tau[2];
double X[2];
double phi;
double l[3];
double alpha = 1.0;

//double q_dyn[6], qd_dyn[6], qdd_dyn[6], tau[6];
struct ForceSensorData *fdata;

int64_t now = 0;



// Create closure to set finished to true after an END or an ABORT
std::function<void(k_api::Base::ActionNotification)> 
check_for_end_or_abort(bool& finished)
{
    return [&finished](k_api::Base::ActionNotification notification)
    {
        std::cout << "EVENT : " << k_api::Base::ActionEvent_Name(notification.action_event()) << std::endl;

        // The action is finished when we receive a END or ABORT event
        switch(notification.action_event())
        {
        case k_api::Base::ActionEvent::ACTION_ABORT:
        case k_api::Base::ActionEvent::ACTION_END:
            finished = true;
            break;
        default:
            break;
        }
    };
}

/*****************************
 * Example related function *
 *****************************/
int64_t GetTickUs()
{
#if defined(_MSC_VER)
    LARGE_INTEGER start, frequency;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    return (start.QuadPart * 1000000)/frequency.QuadPart;
#else
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    return (start.tv_sec * 1000000LLU) + (start.tv_nsec / 1000);
#endif
}

/**************************
 * Example core functions *
 **************************/
void example_move_to_home_position(k_api::Base::BaseClient* base)
{
    // Make sure the arm is in Single Level Servoing before executing an Action
    auto servoingMode = k_api::Base::ServoingModeInformation();
    servoingMode.set_servoing_mode(k_api::Base::ServoingMode::SINGLE_LEVEL_SERVOING);
    base->SetServoingMode(servoingMode);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Move arm to ready position
    std::cout << "Moving the arm to a safe position" << std::endl;
    auto action_type = k_api::Base::RequestedActionType();
    action_type.set_action_type(k_api::Base::REACH_POSE);
    auto action_list = base->ReadAllActions(action_type);
    auto action_handle = k_api::Base::ActionHandle();
    action_handle.set_identifier(0);
    for (auto action : action_list.action_list()) 
    {
        std::cout << action.name() << ",";
        if (action.name() == "TableHome") //changed!!
        {
            action_handle = action.handle();
        }
    }

    if (action_handle.identifier() == 0) 
    {
        std::cout << "Can't reach safe position, exiting" << std::endl;
    } 
    else 
    {
        bool action_finished = false; 
        // Notify of any action topic event
        auto options = k_api::Common::NotificationOptions();
        auto notification_handle = base->OnNotificationActionTopic(
            check_for_end_or_abort(action_finished),
            options
        );

        base->ExecuteActionFromReference(action_handle);

        while(!action_finished)
        { 
            std::this_thread::sleep_for(ACTION_WAITING_TIME);
        }

        base->Unsubscribe(notification_handle);
    }
}

bool admittanceControl(k_api::Base::BaseClient* base, k_api::BaseCyclic::BaseCyclicClient* base_cyclic)
{
    bool return_status = true;

    kinovaDynamics_initialize();

    fdata = (ForceSensorData*)calloc(1,sizeof *fdata);

    initForceSensorUDP(fdata);
    tareForceSensor(fdata);
    sleep(2);

    int actuatorIndexs[3] = {1,3,5};

    z = (double*)calloc(DURATION*1000,sizeof(double));
    Fsense = (double*)calloc(DURATION*1000,sizeof(double));
    Fest = (double*)calloc(DURATION*1000,sizeof(double));

    for(int i = 0; i < 3; i++)
    {
        q[i] = (double*)calloc(DURATION*1000,sizeof(double));
        qd[i] = (double*)calloc(DURATION*1000,sizeof(double));
    }
    
    
    //logfile 
    //openLogFile(&outputFile);

    // Move arm to ready position
    example_move_to_home_position(base);

    k_api::BaseCyclic::Feedback base_feedback;
    k_api::BaseCyclic::Command  base_command;

    std::vector<float> commands;

    auto servoingMode = k_api::Base::ServoingModeInformation();

    int timer_count = 0;

    int64_t last = 0;

    int timeout = 0;

    std::cout << "Initializing the arm for velocity low-level control example" << std::endl;
    try
    {
        // Set the base in low-level servoing mode
        servoingMode.set_servoing_mode(k_api::Base::ServoingMode::LOW_LEVEL_SERVOING);
        base->SetServoingMode(servoingMode);
        base_feedback = base_cyclic->RefreshFeedback();

        int actuator_count = base->GetActuatorCount().count();

        // Initialize each actuator to its current position
        for(int i = 0; i < actuator_count; i++)
        {
            commands.push_back(base_feedback.actuators(i).position());
            base_command.add_actuators()->set_position(base_feedback.actuators(i).position());
            if(i < 3)
            {
                q[i][0] = PI*base_feedback.actuators(actuatorIndexs[i]).position()/180.0;
                q_cur[i] = q[i][0];
                q_prev[i][0] = q(i);
            }
            
        }

        //TODO set l, set x0
        simplifiedForwardKinematics(&q_cur, &X, &phi, &l);
        xd = 0;

        VectorXd Xv(2);
        Xv << xd, 0;
        VectorXd X0(2);
        X0 << 0.05, 0;
        VectorXd Fext(1);
        Fext << 0.0;

        VectorXd K(1);
        K << 30.0;
        VectorXd D(1);
        D << 200.0;
        VectorXd M(1);
        M << 1.0;

        Eigen::MatrixXd A(2, 2);
        Eigen::MatrixXd Ad(2, 2);
        Eigen::VectorXd Bd(2);

        A = constructA(K, D, M, 1);
        Ad = discretizeA(A, 1);
        Bd = discretizeB(M, Ad, A, 1);

        std::cout << Ad << std::endl;

        //double x0 = X.p(ctlAxis); //change to joint limit (avoid collisions!)
        
        time_duration = DURATION;//4*PI/(1000.0*rate); //do 2 cycles
        std::cout << "time duration:" << time_duration << std::endl;
        //std::cout << x0 << "," << x1 << "," << X.p(0) << std::endl;

        // Define the callback function used in Refresh_callback
        auto lambda_fct_callback = [](const Kinova::Api::Error &err, const k_api::BaseCyclic::Feedback data)
        {
            for(int i = 0; i < 3; i ++)
            {
                q[i][timer_count] = PI*data.actuators(actuatorIndexs[i]).position()/180.0; //transfer feedback to kdl 
                dq[i][timer_count] = PI*data.actuators(actuatorIndexs[i]).velocity()/180.0; 
                iact[timeer_count] = data.actuators(actuatorIndexs[i]).current_motor();
            }
            
        };

        startForceSensorStream(fdata);

        // Real-time loop
        while(timer_count < (time_duration * 1000))
        {
            now = GetTickUs();
            if(now - last > 1000)
            {

                for(int i = 0; i < 3; i++)
                {
                    q_cur[i] = q[i][timer_count];
                    q_prev[i] = q_cur[i];
                    qd_cur[i] = dq[i][timer_count];
                    qdd_cur[i] = 0.0;
                }

                if(alpha > 0)
                {
                    readForceSensor(fdata);
                    Fsense[timer_count] = -fdata->F[2];
                } 
                if(alpha < 1)
                {
                    kinovaInverseDynamics(q_cur, qd_cur, qdd_cur, tau);
                    torqueToForce(&tau, &q_cur, &l, &Fest[timer_count]);
                } 

                Fext << alpha*Fsense[timer_count] + (1.0-alpha)*Fest[timer_count];
                Xv = virtualTrajectory(Ad, Bd, Fext, Xv, X0);
                z[timer_count] = Xv(0);

                X[0] = 1.0;
                X[1] = z[timer_count];
                
                simplifiedInverseKinematics(&q_cur, &X, &phi, &l);

                
                if(checkCommandAngleSimplified(q_cur, q_prev, 0.08)) 
                {
                    for(int i = 0; i < 3; i++)
                    {
                        if(q_cur[i] < 0.0) q_cur[i] = q_cur[i] + 2*PI; 
  
                        base_command.mutable_actuators(actuatorIndexs[i])->set_position(fmod(180.0*q[i]/PI, 360.0f));
                        
                    }
                }
                else
                {
                    servoingMode.set_servoing_mode(k_api::Base::ServoingMode::SINGLE_LEVEL_SERVOING);
                    base->SetServoingMode(servoingMode);

                    std::cout << "Exceeded command threshold" << std::endl;
                    
                    for(int i = 0; i < 3; i++)
                    {
                        std::cout<<q_cur[i]<<","<<q_prev[i]<<std::endl;
                    }

                    // Wait for a bit
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

                    return return_status;
                }

                try
                {
                    base_cyclic->Refresh_callback(base_command, lambda_fct_callback, 0);
                }
                catch(...)
                {
                    timeout++;
                }
                
                timer_count++;
                last = GetTickUs();
            }
        }
    }
    catch (k_api::KDetailedException& ex)
    {
        std::cout << "Kortex error: " << ex.what() << std::endl;
        return_status = false;
    }
    catch (std::runtime_error& ex2)
    {
        std::cout << "Runtime error: " << ex2.what() << std::endl;
        return_status = false;
    }
 
    // Set back the servoing mode to Single Level Servoing
    servoingMode.set_servoing_mode(k_api::Base::ServoingMode::SINGLE_LEVEL_SERVOING);
    base->SetServoingMode(servoingMode);

 
    writeAllData(&outputFile, q, dq, z, Fsense, Fest, iact, DURATION*1000);

    // Wait for a bit
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    kinovaDynamics_terminate();

    return return_status;
}

int main(int argc, char **argv)
{
    auto parsed_args = ParseExampleArguments(argc, argv);

    // Create API objects
    auto error_callback = [](k_api::KError err){ cout << "_________ callback error _________" << err.toString(); };
    
    auto transport = new k_api::TransportClientTcp();
    auto router = new k_api::RouterClient(transport, error_callback);
    transport->connect(parsed_args.ip_address, PORT);

    auto transport_real_time = new k_api::TransportClientUdp();
    auto router_real_time = new k_api::RouterClient(transport_real_time, error_callback);
    transport_real_time->connect(parsed_args.ip_address, PORT_REAL_TIME);

    // Set session data connection information
    auto create_session_info = k_api::Session::CreateSessionInfo();
    create_session_info.set_username(parsed_args.username);
    create_session_info.set_password(parsed_args.password);
    create_session_info.set_session_inactivity_timeout(60000);   // (milliseconds)
    create_session_info.set_connection_inactivity_timeout(2000); // (milliseconds)

    // Session manager service wrapper
    std::cout << "Creating sessions for communication" << std::endl;
    auto session_manager = new k_api::SessionManager(router);
    session_manager->CreateSession(create_session_info);
    auto session_manager_real_time = new k_api::SessionManager(router_real_time);
    session_manager_real_time->CreateSession(create_session_info);
    std::cout << "Sessions created" << std::endl;

    // Create services
    auto base = new k_api::Base::BaseClient(router);
    auto base_cyclic = new k_api::BaseCyclic::BaseCyclicClient(router_real_time);

    auto isOk = admittanceControl(base, base_cyclic);
    if (!isOk)
    {
        std::cout << "There has been an unexpected error in example_cyclic_armbase() function." << std::endl;
    }

    // Close API session
    session_manager->CloseSession();
    session_manager_real_time->CloseSession();

    // Deactivate the router and cleanly disconnect from the transport object
    router->SetActivationStatus(false);
    transport->disconnect();
    router_real_time->SetActivationStatus(false);
    transport_real_time->disconnect();

    // Destroy the API
    delete base;
    delete base_cyclic;
    delete session_manager;
    delete session_manager_real_time;
    delete router;
    delete router_real_time;
    delete transport;
    delete transport_real_time;
}