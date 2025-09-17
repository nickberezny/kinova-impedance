#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream> 
#include <sys/stat.h> 
#include <cassert>
#include <cstdlib>

#include <BaseClientRpc.h>
#include <BaseCyclicClientRpc.h>

#include "../include/dataLogger.h"

#define NMJNTS 7;

namespace k_api = Kinova::Api;


void writeDataToLog(std::ofstream * outputFile, const k_api::BaseCyclic::Feedback data)
{
    if ((*outputFile).is_open()) {
        for(int i = 0; i < 7; i++)
        {
            (*outputFile) << data.actuators(i).position() << ",";
            (*outputFile) << data.actuators(i).velocity() << ",";
            (*outputFile) << data.actuators(i).torque() << ",";
            (*outputFile) << data.actuators(i).current_motor() << ",";
        }
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

        for(int i=0; i < 7; i++)
        {
            (*outputFile) << "J" << std::to_string(i) << ",,,,";
        }
        (*outputFile) << std::endl;
        for(int i=0; i < 7; i++)
        {
            (*outputFile) << "x,dx,tau,i,";
        }
        (*outputFile) << std::endl;

    }


}
