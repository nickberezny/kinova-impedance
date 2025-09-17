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
#include <stdio.h>
#include <iostream>

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