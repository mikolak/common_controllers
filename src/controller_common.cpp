// Copyright 2014 WUT
/*
 * controller_common.cpp
 *
 *  Created on: 25 sty 2014
 *      Author: konrad
 */
#include <rtt/Component.hpp>

#include "controller_common/vector_concate.h"
#include "controller_common/vector_split.h"
#include "controller_common/decimator.h"
#include "controller_common/cartesian_impedance.h"
#include "cartesian_interpolator.h"
#include "cartesian_trajectory_action.h"
#include "joint_limit_avoidance.h"
#include "robot_mass_matrix.h"
#include "mass_test.h"
#include "cartesian_impedance_interpolator.h"
#include "cartesian_impedance_action.h"
#include "pose_transform.h"
#include "tf_publisher.h"
#include "torque_publisher.h"
#include "joint_impedance.cpp"

typedef VectorConcate<2> VectorConcate2;
typedef VectorConcate<3> VectorConcate3;
typedef VectorConcate<4> VectorConcate4;

ORO_LIST_COMPONENT_TYPE(VectorConcate2)
ORO_LIST_COMPONENT_TYPE(VectorConcate3)
ORO_LIST_COMPONENT_TYPE(VectorConcate4)

typedef VectorSplit<2> VectorSplit2;
typedef VectorSplit<3> VectorSplit3;
typedef VectorSplit<4> VectorSplit4;

ORO_LIST_COMPONENT_TYPE(VectorSplit2)
ORO_LIST_COMPONENT_TYPE(VectorSplit3)
ORO_LIST_COMPONENT_TYPE(VectorSplit4)

typedef Decimator<geometry_msgs::Wrench> DecimatorWrench;

ORO_LIST_COMPONENT_TYPE(DecimatorWrench)

ORO_LIST_COMPONENT_TYPE(CartesianImpedance)

ORO_LIST_COMPONENT_TYPE(CartesianInterpolator)

ORO_LIST_COMPONENT_TYPE(CartesianTrajectoryAction)

ORO_LIST_COMPONENT_TYPE(JointLimitAvoidance)

ORO_LIST_COMPONENT_TYPE(RobotMassMatrix)

ORO_LIST_COMPONENT_TYPE(MassTest)

ORO_LIST_COMPONENT_TYPE(CartesianImpedanceInterpolator)

ORO_LIST_COMPONENT_TYPE(CartesianImpedanceAction)

ORO_LIST_COMPONENT_TYPE(PoseTransform)

ORO_LIST_COMPONENT_TYPE(TfPublisher)

ORO_LIST_COMPONENT_TYPE(TorquePublisher)

ORO_LIST_COMPONENT_TYPE(JointImpedance)

ORO_CREATE_COMPONENT_LIBRARY()

