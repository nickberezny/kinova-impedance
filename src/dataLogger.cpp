#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream> 
#include <sys/stat.h> 
#include <cassert>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <BaseClientRpc.h>
#include <BaseCyclicClientRpc.h>

#include "../include/dataLogger.h"
#include "../include/forceSensor.h"

#define NMJNTS 7;

namespace k_api = Kinova::Api;


void writeDataToLog(std::ofstream * outputFile, const k_api::BaseCyclic::Feedback data, struct ForceSensorData * forceSensorData, int64_t now)
{
    if ((*outputFile).is_open()) {

        (*outputFile) << now << ",";

        for(int i = 0; i < 7; i++)
        {
            (*outputFile) << data.actuators(i).position() << ",";
            (*outputFile) << data.actuators(i).velocity() << ",";
            (*outputFile) << data.actuators(i).torque() << ",";
            (*outputFile) << data.actuators(i).current_motor() << ",";
        }

        (*outputFile) << forceSensorData->F[0] << "," << forceSensorData->F[1] << "," << forceSensorData->F[2] << ",";
        (*outputFile) << forceSensorData->T[0] << "," << forceSensorData->T[1] << "," << forceSensorData->T[2];

        (*outputFile) << std::endl;
    }

}


void openLogFile(std::ofstream * outputFile)
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "log/%d-%m-%Y-%H-%M-%S");
    auto str = oss.str();

    int check = mkdir(str.c_str(), 0777); //make directory with todays date 

    oss << "/log.txt";

    (*outputFile).open(oss.str()); //open log file

    if ((*outputFile).is_open()) {

        (*outputFile) << ",";
        for(int i=0; i < 7; i++)
        {
            (*outputFile) << "J" << std::to_string(i) << ",,,,";
        }
        (*outputFile) << std::endl << "t,";
        for(int i=0; i < 7; i++)
        {
            (*outputFile) << "x,dx,tau,i,";
        }
        (*outputFile) << std::endl;

    }


}
