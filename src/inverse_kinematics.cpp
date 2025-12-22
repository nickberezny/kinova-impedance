// Copyright  (C)  2007  Francois Cauwe <francois at cauwe dot org>
 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
 
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

#include "kdl/chainiksolvervel_pinv.hpp"
#include <stdio.h>
#include <iostream>
#include <cmath> 

#include "../include/structs.h"
#include "../include/inverse_kinematics.h"
 
using namespace KDL;

KDL::Chain loadKDLChain(std::string urdf_path)
{
    KDL::Tree tree_;
    KDL::Chain chain_;
    //std::unique_ptr<KDL::ChainIkSolverPos_LMA> solver;

    kdl_parser::treeFromFile(urdf_path, tree_);
    tree_.getChain("base_link", "EndEffector_Link", chain_);

    std::cout << "chain nb joints:  " << chain_.getNrOfJoints() << std::endl;

    return chain_;
}

void torqueToForce(double tau[2], double q[3], double l[3], double *Fz)
{

    *Fz = tau[0]/(l[0]*cos(q[0])+l[1]*cos(q[1])) + tau[1]/(l[1]*cos(q[1])); 
    return;

}

void forceToTorque(double F, double l[3], double q[3], double * tau, double * offset)
{
    double m2 = l[1]*sin(q[1]-3.14-q[0]);
    double m1 = l[0]*sin(q[0]) + m2;
    tau[0] = F*m1 + offset[0];
    tau[1] = F*m2 + offset[1];
    return;
}


void simplifiedInverseKinematics(double q[3], double X[2], double * phi, double l[3])
{
    double nx,ny,delta,c2,s2,s1,c1;

    nx = X[0] - l[2]*cos(*phi);
    ny = X[1] - l[2]*sin(*phi);
    delta = nx*nx + ny*ny;
    c2 = (delta - l[0]*l[0] - l[1]*l[1])/(2.0*l[0]*l[1]);
    s2 = sqrt(1.0-c2*c2);

    q[1] = atan2(s2,c2);

    s1 = ((l[0]+l[1]*c2)*ny - l[1]*s2*nx)/delta;
    c1 = ((l[0]+l[1]*c2)*nx + l[1]*s2*ny)/delta;



    std::cout << s1 << ",," << c1 << std::endl;

    q[0] = atan2(s1,c1);

    q[2] = *phi - q[0] - q[1];

    return;
}

void simplifiedForwardKinematics(double q[3], double X[2], double * phi, double l[3])
{
    Eigen::MatrixXd H01(4,4);
    H01 << cos(q[0]), -sin(q[0]), 0, l[0]*cos(q[0]),
    sin(q[0]), cos(q[0]), 0, l[0]*sin(q[0]),
    0,0,1,0,
    0,0,0,1;
    

    Eigen::MatrixXd H12(4,4);
    H12 << cos(q[1]), -sin(q[1]), 0, l[1]*cos(q[1]),
        sin(q[1]), cos(q[1]), 0, l[1]*sin(q[1]),
        0,0,1,0,
        0,0,0,1;
    

    Eigen::MatrixXd H23(4,4);
    H23 << cos(q[2]), -sin(q[2]), 0, l[2]*cos(q[2]),
        sin(q[2]), cos(q[2]), 0, l[2]*sin(q[2]),
        0,0,1,0,
        0,0,0,1;
    

    Eigen::MatrixXd H03(4,4);
    Eigen::MatrixXd H02(4,4);

    H03=H01*H12*H23;  
    //H02=H01*H12;    

    Eigen::VectorXd P3(2);
    P3 << H03(0,3), H03(1,3);

    std::cout << P3(0) <<"," << P3(1) << std::endl;

    return;
}


/*
int main( int argc, char** argv )
{

    KDL::JntArray q_prev(7);
    KDL::JntArray q_cur(7);
    KDL::JntArray dq_cur(7);
    KDL::Jacobian jac(7);

    const std::string urdf = "/home/nick/Documents/kinova_impedance/URDF/GEN3_URDF_V12.urdf";

    KDL::Chain chain_;
    chain_ = loadKDLChain(urdf);
    kdl_solvers solvers(chain_);

    KDL::Frame X; 
    X.p = KDL::Vector(0.3, -0.8, 0.5);
    X.M = KDL::Rotation::RotX(KDL::PI*3.0/4.0);

    for(int i = 0; i < 7; i++)
    {
      q_prev(i) = 0.5;
    }

    solvers.IK_solver->CartToJnt(q_prev, X, q_cur);

    for(int i = 0; i < 7; i++)
    {
      //q_prev(i) = q_cur(i);
      std::cout << q_cur(i) << ",";
    }

    std::cout << std::endl;
   
    solvers.Jac_solver->JntToJac(q_cur, jac);

    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j<7; j++)
        {
            std::cout << jac(i,j) << ",";
        }
        std::cout << std::endl;
    }
    
    
 

}
*/