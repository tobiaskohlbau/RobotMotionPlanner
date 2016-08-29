//-------------------------------------------------------------------------//
//
// Copyright 2016 Sascha Kaden
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-------------------------------------------------------------------------//

#include <robot/SerialRobot.h>

#include <core/Logging.h>
#include <core/Utilities.h>

using namespace rmpl;
using std::shared_ptr;

/*!
*  \brief      Constructor of the class RobotBase
*  \author     Sascha Kaden
*  \param[in]  name of the robot
*  \param[in]  type of the robot
*  \param[in]  dimensions of the robot
*  \param[in]  number of joints of the robot
*  \date       2016-06-30
*/
SerialRobot::SerialRobot(std::string name, CollisionType type, unsigned int dim) : RobotBase(name, type, RobotType::serial, dim) {
}

/*!
*  \brief      Create transformation matrix from the passed D-H parameter and the joint angle
*  \author     Sascha Kaden
*  \param[in]  D-H alpha parameter
*  \param[in]  D-H a parameter
*  \param[in]  D-H d parameter
*  \param[in]  joint angle
*  \param[out] transformation matrix
*  \date       2016-07-07
*/
Eigen::Matrix4f SerialRobot::getTrafo(float alpha, float a, float d, float q) {
    float sinAlpha = sin(alpha);
    float cosAlpha = cos(alpha);
    float sinQ = sin(q);
    float cosQ = cos(q);

    Eigen::Matrix4f T = Eigen::Matrix4f::Zero(4, 4);
    T(0, 0) = cosQ;
    T(0, 1) = -sinQ * cosAlpha;
    T(0, 2) = sinQ * sinAlpha;
    T(0, 3) = a * cosQ;
    T(1, 0) = sinQ;
    T(1, 1) = cosQ * cosAlpha;
    T(1, 2) = -cosQ * sinAlpha;
    T(1, 3) = a * sinQ;
    T(2, 1) = sinAlpha;
    T(2, 2) = cosAlpha;
    T(2, 3) = d;
    T(3, 3) = 1;
    return T;
}

/*!
*  \brief      Compute TCP pose from transformation matrizes and the basis pose
*  \author     Sascha Kaden
*  \param[in]  transformation matrizes
*  \param[in]  basis pose
*  \param[out] TCP pose
*  \date       2016-07-07
*/
Vec<float> SerialRobot::getTcpPosition(const std::vector<Eigen::Matrix4f> &trafos) {
    // multiply these matrizes together, to get the complete transformation
    // T = A1 * A2 * A3 * A4 * A5 * A6
    Eigen::Matrix4f robotToTcp = trafos[0];
    for (int i = 1; i < 6; ++i)
        robotToTcp *= trafos[i];

    Eigen::Matrix4f basisToTcp = m_poseMat * robotToTcp;

    return Utilities::poseMatToVec(basisToTcp);
}

/*!
*  \brief      Return MeshContainer from joint by index
*  \author     Sascha Kaden
*  \param[in]  joint index
*  \param[out] MeshContainer
*  \date       2016-08-25
*/
shared_ptr<MeshContainer> SerialRobot::getMeshFromJoint(unsigned int jointIndex) {
    if (jointIndex < m_joints.size()) {
        return m_joints[jointIndex].getMesh();
    } else {
        Logging::error("Joint index larger than joint size", this);
        return nullptr;
    }
}

/*!
*  \brief      Return pqp mode vector from joints
*  \author     Sascha Kaden
*  \param[out] vector of pqp models
*  \date       2016-08-25
*/
std::vector<shared_ptr<PQP_Model>> SerialRobot::getJointPqpModels() {
    std::vector<shared_ptr<PQP_Model>> models;
    for (auto joint : m_joints)
        models.push_back(joint.getMesh()->getPqp());
    return models;
}

/*!
*  \brief      Return fcl model vector from joints
*  \author     Sascha Kaden
*  \param[out] vector of fcl models
*  \date       2016-08-25
*/
std::vector<shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>>> SerialRobot::getJointFclModels() {
    std::vector<shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>>> models;
    for (auto joint : m_joints)
        models.push_back(joint.getMesh()->getFcl());
    return models;
}

/*!
*  \brief      Return number of the joints from the robot
*  \author     Sascha Kaden
*  \param[out] number of joints
*  \date       2016-06-30
*/
unsigned int SerialRobot::getNbJoints() {
    return m_joints.size();
}
