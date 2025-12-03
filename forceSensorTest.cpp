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

#include "../include/dataLogger.h"
#include "../include/forceSensor.h"
#include "../include/structs.h"

const double PI = 3.14159265358979323846;

int main() {

    struct ForceSensorData *fdata;

    fdata = (ForceSensorData*)calloc(1,sizeof *fdata);

    std::chrono::microseconds elapsed_ms;

    // Simulate some work
    



    initForceSensorUDP(fdata);
    tareForceSensor(fdata);
    sleep(2);
    startForceSensorStream(fdata);


    for(int i = 0; i < 10000; i++)
    {

        auto start = std::chrono::high_resolution_clock::now();

        readFroceSensor(fdata);
        //std::cout<<fdata->F[1]<<std::endl;

        auto end = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::microseconds(1100)); 

        elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Elapsed time in milliseconds: " << elapsed_ms.count() << " ms\n";

    }

    return 0;

}