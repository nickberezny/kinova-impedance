#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream> 
#include <sys/stat.h> 
#include <cassert>
#include <cstdlib>

#include <BaseCyclicClientRpc.h>

#include <chrono>

#include "include/dataLogger.h"

int main( int argc, char** argv )
{

	std::ofstream outputFile; //log file
	double x = 1.239;
	openLogFile(&outputFile);
	
	auto lastFrameTime = std::chrono::steady_clock::now();
    
    for(int i = 0; i <7; i++)
    {
    	outputFile << x << "," << x << "," << x << ",";
    }
	
    auto currentFrameTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> deltaTime = currentFrameTime - lastFrameTime;
    std::cout << "Delta Time: " << deltaTime.count() << " seconds" << std::endl;

	

}