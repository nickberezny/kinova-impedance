#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>   


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

#include "../include/inverse_kinematics.h"

const double PI = 3.14159265358979323846;

int main() {

    double q[3] = {1,2,3};
    double z = 0.31;
    double X[2] = {z-0.274,0.475};
    //double X[2] = {0.3109+0.436,0.168};
    double phi = -PI;
    //double l[3] = {0.42,0.3143,0.1674};
    double l[3] = {0.4368,0.3109,0.169};
    
    simplifiedInverseKinematics(q, X, &phi, l);
    std::cout << 180.0*q[0]/PI << "," << 180.0*q[1]/PI << "," << 180.0*q[2]/PI << std::endl;


    simplifiedForwardKinematics(q, X, &phi, l);

    /*
    for(double zd = 0.1; zd < 0.4; zd = zd + 0.025)
    {
        X[1] = zd-0.28;
        
    }
*/
    

    return 0;

}