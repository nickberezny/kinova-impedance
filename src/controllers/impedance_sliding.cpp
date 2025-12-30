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
#include <signal.h>

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
#include <ActuatorConfigClientRpc.h>

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

#include <chrono>

#include "utilities.h"
#include "../include/structs.h"
#include "../include/inverse_kinematics.h"
#include "../include/dataLogger.h"
#include "../include/safety.h"
#include "../include/forceSensor.h"
#include "../include/admittance.h"
#include "../include/inverse_dynamics.h"
#include "../include/filters.hpp"
#include "../include/computed_torque.hpp"

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

#define DURATION 30             // Network timeout (seconds)

float velocity = 20.0f;         // Default velocity of the actuator (degrees per seconds)
float time_duration = DURATION; // Duration of the example (seconds)

// Waiting time during actions
const auto ACTION_WAITING_TIME = std::chrono::seconds(1);
const double PI = 3.14159265358979323846;

std::ofstream outputFile; //log file

int timer_count = 0;
int actuatorIndexs[3] = {1,3,5};

double *q[3];
double *qd[3];
double *iact[3];
double *tact[3];
double *z;
double *dz;
double *ddz;
double *Fsense;
double *Fest;


double q_cur[3];
double q_prev[3];
double qd_cur[3];
double qdd_cur[3];
double tau[2];
double phi = -PI;
double l[3] = {0.4368,0.3109,0.169};
double alpha = 0.6;
double tau_off[2] = {19.5,-4.5}; //TODO Change

KDL::JntArray q_kdl(7);
KDL::JntArray q_kdl_prev(7);
KDL::Frame X;

//double q_dyn[6], qd_dyn[6], qdd_dyn[6], tau[6];
struct ForceSensorData *fdata;

int64_t now = 0;

k_api::Base::ServoingModeInformation servoingMode;
k_api::Base::BaseClient *base;
k_api::ActuatorConfig::ControlModeInformation control_mode_message;
k_api::ActuatorConfig::ActuatorConfigClient *actuator_config;

void my_handler(int s)
{
    printf("Caught signal %d\n", s);
    control_mode_message.set_control_mode(k_api::ActuatorConfig::ControlMode::POSITION);
    actuator_config->SetControlMode(control_mode_message, 2);
    actuator_config->SetControlMode(control_mode_message, 4);

    servoingMode.set_servoing_mode(k_api::Base::ServoingMode::SINGLE_LEVEL_SERVOING);
    base->SetServoingMode(servoingMode);
    exit(1);
}


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

bool admittanceControl(k_api::Base::BaseClient* base, k_api::BaseCyclic::BaseCyclicClient* base_cyclic,k_api::ActuatorConfig::ActuatorConfigClient* actuator_config)
{
    bool return_status = true;


    //kinovaDynamics_initialize();

    fdata = (ForceSensorData*)calloc(1,sizeof *fdata);

    initForceSensorUDP(fdata);
    tareForceSensor(fdata);
    sleep(2);

    VectorXd dzvec(8);
    VectorXd dzfilt(7);
    dzvec << 0,0,0,0,0,0,0,0;
    dzfilt << 0,0,0,0,0,0,0;

    VectorXd ddzvec(8);
    VectorXd ddzfilt(7);
    ddzvec << 0,0,0,0,0,0,0,0;
    ddzfilt << 0,0,0,0,0,0,0;

    VectorXd afilt(7);
    afilt <<-6.43532475688657790158,17.76978864520641820945,-27.29100833212815757634,25.17605177335131116934,-13.94983800505135285164,4.29857409735324225153,-0.56824304536622272099;
    VectorXd bfilt(8);
    bfilt << 0.00000000294123952020,0.00000002058867664137,0.00000006176602992412,0.00000010294338320686,0.00000010294338320686,0.00000006176602992412,0.00000002058867664137,0.00000000294123952020;



    const std::string urdf = "/home/nick/Documents/Github/kinova-impedance/URDF/GEN3_URDF_V12.urdf";

    KDL::Chain chain_;
    chain_ = loadKDLChain(urdf);
    kdl_solvers solvers(chain_);
    solvers.FK_solver_pos = std::make_unique<KDL::ChainFkSolverPos_recursive>(chain_);
    solvers.IK_solver = std::make_unique<KDL::ChainIkSolverPos_LMA>(chain_);

    double z0 = 0.095;
    double Md = 3.0;
    double Bd = 300.0;
    double Kd = 0.0; //200.0;
    double Fic = 0.0;
    double tor_cmd[2] = {0.0,0.0};
    double zint = 0.0;
    double Fint = 0.0;
    double zs = 0.0; // should be set to something else?

    z = (double*)calloc(DURATION*1000,sizeof(double));
    dz = (double*)calloc(DURATION*1000,sizeof(double));
    ddz = (double*)calloc(DURATION*1000,sizeof(double));

    Fsense = (double*)calloc(DURATION*1000,sizeof(double));
    Fest = (double*)calloc(DURATION*1000,sizeof(double));

    for(int i = 0; i < 3; i++)
    {
        q[i] = (double*)calloc(DURATION*1000,sizeof(double));
        qd[i] = (double*)calloc(DURATION*1000,sizeof(double));
        iact[i] = (double*)calloc(DURATION*1000,sizeof(double));
        tact[i] = (double*)calloc(DURATION*1000,sizeof(double));
    }
    
    
    //logfile 
    openLogFile(&outputFile);

    // Move arm to ready position
    //example_move_to_home_position(base);

    k_api::BaseCyclic::Feedback base_feedback;
    k_api::BaseCyclic::Command  base_command;

    std::vector<float> commands;

    servoingMode = k_api::Base::ServoingModeInformation();

    

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
                q_prev[i] = q_cur[i];
            }

            q_kdl(i) = PI*base_feedback.actuators(i).position()/180.0;
            q_kdl_prev(i) = q_kdl(i);
        }

        base_feedback = base_cyclic->Refresh(base_command);
        
        // Set first actuator in torque mode now that the command is equal to measure
        control_mode_message = k_api::ActuatorConfig::ControlModeInformation();
        control_mode_message.set_control_mode(k_api::ActuatorConfig::ControlMode::TORQUE);


        //TODO: set actuator 2 (id = 2) and 4 (id = 4)
        int first_actuator_device_id = 2;
        actuator_config->SetControlMode(control_mode_message, first_actuator_device_id);
        int second_actuator_device_id = 4;
        actuator_config->SetControlMode(control_mode_message, second_actuator_device_id);


        solvers.FK_solver_pos->JntToCart(q_kdl, X);


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
                qd[i][timer_count] = PI*data.actuators(actuatorIndexs[i]).velocity()/180.0; 
                iact[i][timer_count] = data.actuators(actuatorIndexs[i]).current_motor();
                tact[i][timer_count] = data.actuators(actuatorIndexs[i]).torque();
            }

            for(int i = 0; i < 7; i ++) q_kdl(i) = PI*data.actuators(i).position()/180.0;

            
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
                    qd_cur[i] = qd[i][timer_count];
                    qdd_cur[i] = 0.0;
                }

                solvers.FK_solver_pos->JntToCart(q_kdl_prev, X);

                z[timer_count] = X.p(2);
                if(timer_count>1) dz[timer_count] = (z[timer_count] - z[timer_count-1])/0.001;
                butterworth(&dz[timer_count], &dz[timer_count], dzvec, dzfilt, afilt, bfilt);
                if(timer_count>2) ddz[timer_count] = (dz[timer_count] - dz[timer_count-1])/0.001;
                

                readForceSensor(fdata);
                Fsense[timer_count] = -fdata->F[2];

                //Fic = -Md*ddz[timer_count] - Bd*dz[timer_count]; //- Kd*(z[timer_count]-z0);

                //forceToTorque

                slidingSurface(&z[timer_count], &z0, &zs, &dz[timer_count], &Fsense[timer_count], &Kd, &Bd, &Md, &zint, &Fint); 
                SMC(&dz[timer_count], &zs, 2.0, &Fic);

                forceToTorque(Fic, l, q_cur, tor_cmd, tau_off);
                //check tor levels!

                if(tor_cmd[0]>-17.0) tor_cmd[0] = -17.0;
                if(tor_cmd[0]<-22.0) tor_cmd[0] = -22.0;

                if(tor_cmd[1]>8.0) tor_cmd[1] = 8.0;
                if(tor_cmd[1]<1.0) tor_cmd[1] = 1.0;

                std::cout << tor_cmd[0] << ", " << tor_cmd[1] << "," << Fic << "," << Fsense[timer_count] << "," << z[timer_count]<< std::endl;
                
                if(checkCommandAngle(q_kdl.data, q_kdl_prev.data, 7, 0.02, 0.02) && z[timer_count] > 0.087 && z[timer_count] < 0.14)
                {
                    for(int i = 0; i < 7; i++) q_kdl_prev(i) = q_kdl(i);

                    //
                    base_command.mutable_actuators(1)->set_position(q_kdl(1)*180.0/PI);
                    base_command.mutable_actuators(1)->set_torque_joint(tor_cmd[0]);  //-tau_off[0] + tor_cmd[0]);
                    base_command.mutable_actuators(3)->set_position(q_kdl(3)*180.0/PI);
                    base_command.mutable_actuators(3)->set_torque_joint(4.5);//-tau_off[1] + tor_cmd[1]);
                }
                else
                {
                    control_mode_message.set_control_mode(k_api::ActuatorConfig::ControlMode::POSITION);
                    actuator_config->SetControlMode(control_mode_message, 2);
                    actuator_config->SetControlMode(control_mode_message, 4);

                    servoingMode.set_servoing_mode(k_api::Base::ServoingMode::SINGLE_LEVEL_SERVOING);
                    base->SetServoingMode(servoingMode);

                    std::cout << "Exceeded command threshold" << std::endl;
                    
                    for(int i = 0; i < 7; i++)
                    {
                        std::cout<<q_kdl(i)<<","<<q_kdl_prev(i)<<std::endl;
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
    control_mode_message.set_control_mode(k_api::ActuatorConfig::ControlMode::POSITION);
    actuator_config->SetControlMode(control_mode_message, 2);
    actuator_config->SetControlMode(control_mode_message, 4);


    servoingMode.set_servoing_mode(k_api::Base::ServoingMode::SINGLE_LEVEL_SERVOING);
    base->SetServoingMode(servoingMode);

 
    writeAllData(&outputFile, q, qd, z, dz, ddz, Fsense, Fest, iact, tact, DURATION*1000);

    // Wait for a bit
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    //kinovaDynamics_terminate();

    return return_status;
}

int main(int argc, char **argv)
{

    signal(SIGINT, my_handler);


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
    base = new k_api::Base::BaseClient(router);
    auto base_cyclic = new k_api::BaseCyclic::BaseCyclicClient(router_real_time);
    actuator_config = new k_api::ActuatorConfig::ActuatorConfigClient(router);


    auto isOk = admittanceControl(base, base_cyclic, actuator_config);
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