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

#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

#include <stdio.h>
#include <iostream>
#include <chrono>
 
using namespace KDL;
 
int main( int argc, char** argv )
{

    const double PI = 3.14159265358979323846;

    KDL::Tree tree_;
    KDL::Chain chain_;
    KDL::Frame X;
    
    KDL::JntSpaceInertiaMatrix H; 

    std::unique_ptr<KDL::ChainIkSolverPos_LMA> solver_;
    std::unique_ptr<KDL::ChainFkSolverPos_recursive> solverFK_;
    std::unique_ptr<KDL::ChainDynParam> dynParam_;

    const std::string urdf = "/home/nick/Documents/kinova_impedance/URDF/GEN3_URDF_V12.urdf";
    kdl_parser::treeFromFile(urdf, tree_);
    KDL::JntArray q_prev(7);
    KDL::JntArray q_cur(7);
    // Print basic information about the tree
    std::cout << "nb joints:        " << tree_.getNrOfJoints() << std::endl;
    std::cout << "nb segments:      " << tree_.getNrOfSegments() << std::endl;
    std::cout << "root segment:     " << tree_.getRootSegment()->first
              << std::endl;

    tree_.getChain("base_link", "EndEffector_Link", chain_);
    std::cout << "chain nb joints:  " << chain_.getNrOfJoints() << std::endl;

    KDL::Jacobian jac(chain_.getNrOfJoints());
    solver_ = std::make_unique<KDL::ChainIkSolverPos_LMA>(chain_);
    solverFK_ = std::make_unique<KDL::ChainFkSolverPos_recursive>(chain_);
    KDL::ChainJntToJacSolver jac_solver(chain_);
   
   
    const KDL::Rotation Ry(0.0,0.0,1.0,0.0,1.0,0.0,-1.0,0.0,0.0);
    const KDL::Rotation Rz(0.0,-1.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0);

    const KDL::Rotation Rx=KDL::Rotation::RotY(KDL::PI/4.0);

    Eigen::VectorXd Q(7);
    Q << 4.7, 15, 180, 230, 341, 55, 67; //360.0, 15.0, 180.0, 230.0, 0.0, 55.0, 90.0;

    for(int i = 0; i < 7; i++)
    {
        q_prev(i) = 2*PI*Q(i)/360.0;
    }
    
    solverFK_->JntToCart(q_prev, X);

    std::cout << X.p(0) << "," << X.p(1) << "," << X.p(2) << std::endl;

    double alpha, beta, delta; 
    X.M.GetRPY(alpha, beta, delta);

    std::cout << 360.0*alpha/(2*PI) << "," << 360.0*beta/(2*PI)<< "," << 360.0*delta/(2*PI)<< std::endl;

    /*



    // Run IK solver
    solver_->CartToJnt(q_prev, X, q_cur);

    KDL::Frame p_in(Rx,KDL::Vector(0, -0.4, 0.6));
    X = p_in;

    //TIME
    solver_->CartToJnt(q_prev, p_in, q_cur);

    X.p = KDL::Vector(0.3, -1.0, 0.5);
    X.M = KDL::Rotation::RotX(KDL::PI*3.0/4.0);
    //KDL::JntArray q_out(chain_.getNrOfJoints());

    for(int i = 0; i < 7; i++)
    {
      q_prev(i) = q_cur(i);
      std::cout << q_cur(i) << ",";
    }
 /*
    unsigned int i = 0;
    jac_solver.JntToJac(q_cur, jac);

    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j<7; j++)
        {
            std::cout << jac(i,j) << ",";
        }
        std::cout << std::endl;
    }
    */

    //double j1 = jac(1,1);

    //

//dynParam_->JntToMass(q_cur, H);

    
 

}