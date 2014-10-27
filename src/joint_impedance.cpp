// Copyright 2014 WUT
/*
 * joint_limit_avoidance.cpp
 *
 *  Created on: 11 sep 2014
 *      Author: konradb3
 */

#include "joint_impedance.h"

#include <string>

JointImpedance::JointImpedance(const std::string& name) :
  RTT::TaskContext(name, PreOperational), number_of_joints_(0) {
  this->ports()->addPort("JointPosition", port_joint_position_);
  this->ports()->addPort("JointPositionCommand", port_joint_position_command_);
  this->ports()->addPort("JointStiffnessCommand", port_joint_stiffness_command_);
  this->ports()->addPort("JointVelocity", port_joint_velocity_);
  this->ports()->addPort("MassMatrix", port_mass_matrix_);
  this->ports()->addPort("JointTorqueCommand", port_joint_torque_command_);
  this->ports()->addPort("NullSpaceTorqueCommand",
                         port_nullspace_torque_command_);

  this->properties()->addProperty("number_of_joints", number_of_joints_);
  this->properties()->addProperty("initial_stiffness", initial_stiffness_);
  //this->properties()->addProperty("limit_range", limit_range_);
  //this->properties()->addProperty("max_trq", max_trq_);
}

JointImpedance::~JointImpedance() {
}

bool JointImpedance::configureHook() {
  port_joint_position_.getDataSample(joint_position_);
  port_joint_velocity_.getDataSample(joint_velocity_);
  port_mass_matrix_.getDataSample(m_);

  if ((initial_stiffness_.size() != number_of_joints_)) {
    RTT::log(RTT::Error) << "invalid configuration data size" << std::endl;
    return false;
  }
  
  joint_torque_command_.resize(number_of_joints_);
  joint_error_.resize(number_of_joints_);
  nullspace_torque_command_ = Eigen::VectorXd::Zero(number_of_joints_);
  k_.resize(number_of_joints_);
  q_.resizeLike(m_);
  k0_.resizeLike(m_);
  es_ = Eigen::GeneralizedSelfAdjointEigenSolver<Eigen::MatrixXd >(number_of_joints_);

  for (size_t i = 0; i < initial_stiffness_.size(); i++) {
    k_(i) = initial_stiffness_[i];
    if (k_(i) < 0.0) {
      return false;
    }
  }

  port_joint_torque_command_.setDataSample(joint_torque_command_);
  
  return true;
}

bool JointImpedance::startHook() {
  if (port_joint_position_.read(joint_position_command_) == RTT::NoData) {
    return false;
  }
  return true;
}

void JointImpedance::stopHook() {
  for (int i = 0; i < joint_torque_command_.size(); i++) {
    joint_torque_command_(i) = 0.0;
  }
  
  port_joint_torque_command_.write(joint_torque_command_);
}

void JointImpedance::updateHook() {
  port_joint_position_.read(joint_position_);
  port_joint_position_command_.read(joint_position_command_);
  port_joint_stiffness_command_.read(k_);
  port_joint_velocity_.read(joint_velocity_);
  port_nullspace_torque_command_.read(nullspace_torque_command_);

  joint_error_.noalias() = joint_position_command_ - joint_position_;
  joint_torque_command_.noalias() = k_.cwiseProduct(joint_error_);
  
  port_mass_matrix_.read(m_);
  tmpNN_ = k_.asDiagonal();
  es_.compute(tmpNN_, m_);
  q_ = es_.eigenvectors().inverse();
  k0_ = es_.eigenvalues();

  tmpNN_ = k0_.cwiseSqrt().asDiagonal();

  d_.noalias() = 2.0 * q_.adjoint() * 0.7 * tmpNN_ * q_;

  joint_torque_command_.noalias() -= d_ * joint_velocity_;

  joint_torque_command_.noalias() += nullspace_torque_command_;

  port_joint_torque_command_.write(joint_torque_command_);
}

