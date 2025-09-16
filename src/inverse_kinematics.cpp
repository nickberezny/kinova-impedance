// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include <rclcpp/serialization.hpp>

#include "kdl/tree.hpp"
#include "kdl/chain.hpp"
#include "kdl/frames.hpp"
#include "kdl/jntarray.hpp"
#include "kdl/chainiksolverpos_lma.hpp"
#include "kdl_parser/kdl_parser.hpp"
#include "kdl/chainjnttojacsolver.hpp"
#include "kdl/chaindynparam.hpp"

#include "custom_interfaces/msg/joints.hpp"
#include "custom_interfaces/msg/pos.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class InverseKinematics : public rclcpp::Node
{
public:
  InverseKinematics()
  : Node("inverse_kinematics"), count_(0)
  {
    subscription_ = this->create_subscription<custom_interfaces::msg::Pos>(
      "command_pos", 10, std::bind(&InverseKinematics::topic_callback, this, std::placeholders::_1));
    publisher_ = this->create_publisher<custom_interfaces::msg::Joints>("command_q", 10);
    initIK();
  }
  KDL::Tree tree_;
  KDL::Chain chain_;
  KDL::Frame X;
  KDL::Jacobian jac;
  KDL::JntSpaceInertiaMatrix H; 

  custom_interfaces::msg::Joints jointq;
  std::unique_ptr<KDL::ChainIkSolverPos_LMA> solver_;
  std::unique_ptr<KDL::ChainJntToJacSolver> jac_solver_;
  std::unique_ptr<KDL::ChainDynParam> dynParam_;

private:
  void topic_callback(const custom_interfaces::msg::Pos msg) 
  {
    //run IK 
    //pub x

    X.p = KDL::Vector(msg.x, msg.y, msg.z);
    X.M = KDL::Rotation::RotX(KDL::PI*3.0/2.0);
    KDL::JntArray q_prev(7);
    KDL::JntArray q_cur(7);
    //KDL::JntArray q_out(chain_.getNrOfJoints());
    // Run IK solver
    solver_->CartToJnt(q_prev, X, q_cur);
    jac_solver_->JntToJac(q_cur, jac);
    dynParam_->JntToMass(q_cur, H);

    for(int i = 0; i < 7; i++)
    {
      jointq.q[i] = q_cur(i);
      q_prev(i) = q_cur(i);
    }

    publisher_->publish(jointq);

  }
  void initIK()
  {
    
    // Construct KDL tree from URDF
    const std::string urdf = "/mnt/c/Users/Nick/Documents/ROS/ros2_ws/src/kinova_uic/urdf/GEN3_URDF_V12.urdf";
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

    solver_ = std::make_unique<KDL::ChainIkSolverPos_LMA>(chain_);
    
    
    const KDL::Rotation Ry(0.0,0.0,1.0,0.0,1.0,0.0,-1.0,0.0,0.0);
    const KDL::Rotation Rz(0.0,-1.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0);

    const KDL::Rotation Rx=KDL::Rotation::RotY(KDL::PI/4.0);

    KDL::Frame p_in(Rx,KDL::Vector(0, -0.4, 0.6));
    X = p_in;
    solver_->CartToJnt(q_prev, p_in, q_cur);

    for(int i = 0; i < 7; i++)
    {
      jointq.q[i] = q_cur(i);
      q_prev(i) = q_cur(i);
      //std::cout << q_cur(i) << ",";
    }

    publisher_->publish(jointq);

  }
  rclcpp::Publisher<custom_interfaces::msg::Joints>::SharedPtr publisher_;
  rclcpp::Subscription<custom_interfaces::msg::Pos>::SharedPtr subscription_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<InverseKinematics>());
  rclcpp::shutdown();
  return 0;
}
