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

#include "include/structs.h"
#include "include/inverse_kinematics.h"
 
using namespace KDL;
 
int main( int argc, char** argv )
{

    const double PI = 3.14159265358979323846;

    KDL::Tree tree_;
    KDL::Chain chain_;
    KDL::Frame X;
    
    KDL::JntSpaceInertiaMatrix H(7); 
    KDL::JntArray gravity(7);
    KDL::JntArray cor(7);

    KDL::Vector grav;
    grav(0) = 0;
    grav(1) = 0;
    grav(2) = -9.81; 


    

    std::unique_ptr<KDL::ChainIkSolverPos_LMA> solver_;
    std::unique_ptr<KDL::ChainFkSolverPos_recursive> solverFK_;
    std::unique_ptr<KDL::ChainDynParam> dynParam_;

    const std::string urdf = "/home/nick/Documents/kinova_impedance/URDF/GEN3_URDF_V12.urdf";
    kdl_parser::treeFromFile(urdf, tree_);
    KDL::JntArray q_prev(7);
    KDL::JntArray q_cur(7);
    KDL::JntArray dq(7);

    chain_ = loadKDLChain(urdf);
    kdl_solvers solvers(chain_);

    // Print basic information about the tree
    std::cout << "nb joints:        " << tree_.getNrOfJoints() << std::endl;
    std::cout << "nb segments:      " << tree_.getNrOfSegments() << std::endl;
    std::cout << "root segment:     " << tree_.getRootSegment()->first
              << std::endl;

    //tree_.getChain("base_link", "EndEffector_Link", chain_);
    std::cout << "chain nb joints:  " << chain_.getNrOfJoints() << std::endl;

    KDL::Jacobian jac(chain_.getNrOfJoints());
    solver_ = std::make_unique<KDL::ChainIkSolverPos_LMA>(chain_);
    //solverFK_ = std::make_unique<KDL::ChainFkSolverPos_recursive>(chain_);
    solvers.FK_solver_pos = std::make_unique<KDL::ChainFkSolverPos_recursive>(chain_);
    KDL::ChainJntToJacSolver jac_solver(chain_);
    dynParam_ = std::make_unique<KDL::ChainDynParam>(chain_, grav);
   
    KDL::Wrench Fext;
    KDL::Wrench Fbase;

    KDL::Vector tor;
    tor(0) = 0;
    tor(1) = 0;
    tor(2) = 0;
    KDL::Vector force;
    force(0) = 0;
    force(1) = 0;
    force(2) = 12.34;

    Fext.torque = tor;
    Fext.force = force;

    KDL::Rotation R;
    R.DoRotX(PI/2.0);
    R.DoRotY(0.0);
    R.DoRotZ(PI/2.0);

    KDL::Frame p_in(R,KDL::Vector(0.46, 0.0, 0.44));

    q_prev(0) = 0;
    q_prev(1) = 2*PI*15.0/360.0;
    q_prev(2) = PI;
    q_prev(3) = 2*PI*230.0/360.0;
    q_prev(4) = 0.0;
    q_prev(5) = 2*PI*55.0/360.0;
    q_prev(6) = PI/2.0;



    Eigen::VectorXd Q(7);
    Q << 360.0, 15.017, 180.0, 230.088, 0.0, 55.0-90.0, 90.0; //360.0, 15.017, 180.0, 242.088, 0.0, 55.0, 90.0;

    for(int i = 0; i < 7; i++)
    {
        q_prev(i) = 2*PI*Q(i)/360.0;
    }

    Eigen::VectorXd dQ(7);
    dQ << 10, 10, 10, 87, 10, 10, 10;
    
    for(int i = 0; i < 7; i++)
    {
        dq(i) = 2*PI*dQ(i)/360.0;
    }

    solvers.FK_solver_pos->JntToCart(q_prev, X);
    std::cout << X.p(0) << "," << X.p(1) << "," << X.p(2) << std::endl;

    Fbase = X*Fext;

    for(int i = 0; i < 3; i++)
    {
        std::cout << Fbase.force[i] << ",";
    }

    solver_->CartToJnt(q_prev, X, q_cur);

    for(int i = 0; i < 7; i++)
    {
        std::cout << 360.0*q_cur(i)/(2.0*PI) << ",";
    }
    std::cout << std::endl;

    for(int i = 0; i < 500; i++)
    {
        X.p(0) = 0.46 + 0.0002*(double(i));
        solver_->CartToJnt(q_prev, X, q_cur);

        Fbase = X.Inverse()*Fext;
/*
        for(int i = 0; i < 3; i++)
        {
            std::cout << Fbase.force[i] << ",";
        }

        
        std::cout << 360.0*q_cur(0)/(2.0*PI) << ",";
        std::cout << 360.0*q_cur(1)/(2.0*PI) << ",";
        std::cout << 360.0*q_cur(2)/(2.0*PI) << ",";
        std::cout << 360.0*q_cur(3)/(2.0*PI) << ",";
        std::cout << 360.0*q_cur(4)/(2.0*PI) << ",";
        std::cout << 360.0*q_cur(5)/(2.0*PI) << ",";
        std::cout << 360.0*q_cur(6)/(2.0*PI) << ",";
        
        for(int i = 0; i < 7; i++)
        {
            std::cout << 360.0*q_cur(i)/(2.0*PI) << ",";
        }
        
        std::cout << std::endl;
           */ 
    }

    /*

    std::cout << X.p(0) << "," << X.p(1) << "," << X.p(2) << std::endl;

    double alpha, beta, delta; 
    X.M.GetRPY(alpha, beta, delta);

    std::cout << 360.0*alpha/(2*PI) << "," << 360.0*beta/(2*PI)<< "," << 360.0*delta/(2*PI)<< std::endl;
    */
    /*
    dynParam_->JntToGravity (q_prev, gravity);
    dynParam_->JntToCoriolis (q_prev, dq, cor);
    dynParam_->JntToMass(q_prev, H);

    KDL::Multiply(H, q_prev, q_cur);

    //q_cur.data=H.data*q_prev.data;

    for(int i = 0; i < 7; i++)
    {
        std::cout << cor(i)   << ",";
        std::cout <<gravity(i) << std::endl;
    }

    */
 

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