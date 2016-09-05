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

#include <core/CollisionDetection.h>

#include <core/Logging.h>
#include <core/Utilities.h>

using namespace rmpl;
using std::shared_ptr;

/*!
*  \brief      Constructor of the class CollisionDetection
*  \author     Sascha Kaden
*  \param[in]  VREP Helper
*  \param[in]  RobotType
*  \date       2016-06-30
*/
CollisionDetection::CollisionDetection(const shared_ptr<RobotBase> &robot) : ModuleBase("CollisionDetection") {
    m_robot = robot;
    m_zeroR = Eigen::Matrix3f::Zero(3, 3);
    for (int i = 0; i < 3; ++i) {
        m_zeroR(i, i) = 1;
        m_zeroT(i) = 0;
    }

    if (m_robot->getCollisionType() == CollisionType::twoD)
        m_2DWorkspace = m_robot->get2DWorkspace();
    if (m_robot->getCollisionType() != CollisionType::twoD)
        m_workspace = m_robot->getWorkspace();
}

/*!
*  \brief      Check for collision
*  \author     Sascha Kaden
*  \param[in]  vec
*  \param[out] binary result of collision
*  \date       2016-05-25
*/
bool CollisionDetection::controlVec(const Vec<float> &vec) {
    assert(vec.getDim() == m_robot->getDim());

    if (m_robot->getCollisionType() == CollisionType::twoD)
        return checkPointRobot(vec[0], vec[1]);
    else
        controlCollisionMesh(vec);
}

bool CollisionDetection::controlCollisionMesh(const Vec<float> &vec) {
    if (m_robot->getRobotType() == RobotType::mobile)
        return checkMobileRobot(vec);
    else
        return checkSerialRobot(vec);
}

/*!
*  \brief      Check collision of a trajectory of points
*  \author     Sascha Kaden
*  \param[in]  vector of points
*  \param[out] binary result of collision
*  \date       2016-05-25
*/
bool CollisionDetection::controlTrajectory(std::vector<Vec<float>> &vecs) {
    if (vecs.size() == 0)
        return false;

    assert(vecs[0].getDim() == m_robot->getDim());

    if (m_robot->getCollisionType() == CollisionType::twoD) {
        for (int i = 0; i < vecs.size(); ++i)
            if (checkPointRobot(vecs[i][0], vecs[i][1]))
                return true;
    } else {
        for (int i = 0; i < vecs.size(); ++i)
            if (controlCollisionMesh(vecs[i]))
                return true;
    }
    return false;
}

/*!
*  \brief      Check for 2D PointRobot collision
*  \author     Sascha Kaden
*  \param[in]  x
*  \param[in]  y
*  \param[out] binary result of collision
*  \date       2016-06-30
*/
bool CollisionDetection::checkPointRobot(float x, float y) {
    if (m_2DWorkspace.rows() == -1) {
        Logging::warning("Empty workspace!", this);
        return false;
    }

    if (m_2DWorkspace(x, y) < 80) {
        return true;
    } else {
        return false;
    }
}

/*!
*  \brief      Check for collision of serial robot
*  \author     Sascha Kaden
*  \param[in]  Vec of robot configuration
*  \param[out] binary result of collision
*  \date       2016-09-02
*/
bool CollisionDetection::checkSerialRobot(const Vec<float> &vec) {
    shared_ptr<SerialRobot> robot(std::static_pointer_cast<SerialRobot>(m_robot));

    std::vector<Eigen::Matrix4f> jointTrafos = robot->getJointTrafos(vec);
    Eigen::Matrix4f pose = robot->getPoseMat();
    Eigen::Matrix4f As[jointTrafos.size()];
    As[0] = (pose * jointTrafos[0]);
    for (int i = 1; i < jointTrafos.size(); ++i)
        As[i] = (As[i - 1] * jointTrafos[i]);

    Eigen::Matrix3f poseR;
    Eigen::Vector3f poseT;
    Utilities::decomposeT(pose, poseR, poseT);

    Eigen::Matrix3f rot[jointTrafos.size()];
    Eigen::Vector3f trans[jointTrafos.size()];
    for (int i = 0; i < jointTrafos.size(); ++i)
        Utilities::decomposeT(As[i], rot[i], trans[i]);

    if (m_robot->getCollisionType() == CollisionType::pqp) {
        shared_ptr<PQP_Model> baseMesh = robot->getBase()->m_pqpModel;
        std::vector<shared_ptr<PQP_Model>> models = robot->getJointPqpModels();
        return checkMesh(models, baseMesh, rot, poseR, trans, poseT);
    } else {
        shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>> baseMesh = robot->getBase()->m_fclModel;
        std::vector<shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>>> models = robot->getJointFclModels();
        return checkMesh(models, baseMesh, rot, poseR, trans, poseT);
    }
    return false;
}

bool CollisionDetection::checkMobileRobot(const Vec<float> &vec) {
    return false;
}

/*!
*  \brief      Check for collision with PQP or fcl library
*  \author     Sascha Kaden
*  \param[in]  MeshContainer one
*  \param[in]  MeshContainer two
*  \param[in]  rotation matrix one
*  \param[in]  rotation matrix two
*  \param[in]  translation vector one
*  \param[in]  translation vector two
*  \param[out] binary result of collision
*  \date       2016-07-14
*/
bool CollisionDetection::checkMesh(std::vector<std::shared_ptr<PQP_Model>> &models, std::shared_ptr<PQP_Model> &base,
                                   Eigen::Matrix3f R[], Eigen::Matrix3f &poseR, Eigen::Vector3f t[], Eigen::Vector3f &poseT) {
    // control collision between base and joints
    for (int i = 1; i < m_robot->getDim(); ++i) {
        if (checkPQP(base, models[i], poseR, R[i], poseT, t[i]))
            return true;
    }

    // control collision of the robot joints with themselves
    for (int i = 0; i < m_robot->getDim(); ++i) {
        for (int j = i + 2; j < m_robot->getDim(); ++j) {
            if (checkPQP(models[i], models[j], R[i], R[j], t[i], t[j])) {
#ifdef DEBUG_OUTPUT
                Logging::debug("Collision between link " + std::to_string(i) + " and link " + std::to_string(j), this);
                Eigen::Vector3f r = As[i].block<3, 3>(0, 0).eulerAngles(0, 1, 2);
                Eigen::Vector3f t = As[i].block<3, 1>(0, 3);
                std::cout << "A" << i << ": ";
                std::cout << "Euler angles: " << r.transpose() << "\t";
                std::cout << "Translation: " << t.transpose() << std::endl;
                r = As[j].block<3, 3>(0, 0).eulerAngles(0, 1, 2);
                t = As[j].block<3, 1>(0, 3);
                std::cout << "A" << j << ": ";
                std::cout << "Euler angles: " << r.transpose() << "\t";
                std::cout << "Translation: " << t.transpose() << std::endl << std::endl;
#endif
                return true;
            }
        }
    }

    // control collision with workspace
    if (m_robot->getWorkspace() != nullptr) {
        shared_ptr<PQP_Model> workspace = m_robot->getWorkspace()->getPqp();

        if (checkPQP(workspace, base, m_zeroR, poseR, m_zeroT, poseT))
            return true;

        for (int i = 0; i < m_robot->getDim(); ++i) {
            if (checkPQP(workspace, models[i], m_zeroR, R[i], m_zeroT, t[i])) {
                Logging::debug("Collision between workspace and link " + std::to_string(i), this);
                return true;
            }
        }
    }
    return false;
}

/*!
*  \brief      Check for collision with PQP or fcl library
*  \author     Sascha Kaden
*  \param[in]  MeshContainer one
*  \param[in]  MeshContainer two
*  \param[in]  rotation matrix one
*  \param[in]  rotation matrix two
*  \param[in]  translation vector one
*  \param[in]  translation vector two
*  \param[out] binary result of collision
*  \date       2016-07-14
*/
bool CollisionDetection::checkMesh(std::vector<std::shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>>> &models,
                                   std::shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>> &base, Eigen::Matrix3f R[],
                                   Eigen::Matrix3f &poseR, Eigen::Vector3f t[], Eigen::Vector3f &poseT) {
    // control collision between base and joints
    for (int i = 1; i < m_robot->getDim(); ++i) {
        if (checkFCL(base, models[i], poseR, R[i], poseT, t[i]))
            return true;
    }

    // control collision of the robot joints with themself
    for (int i = 0; i < m_robot->getDim(); ++i) {
        for (int j = i + 2; j < m_robot->getDim(); ++j) {
            if (checkFCL(models[i], models[j], R[i], R[j], t[i], t[j])) {
#ifdef DEBUG_OUTPUT
                Logging::debug("Collision between link " + std::to_string(i) + " and link " + std::to_string(j), this);
                Eigen::Vector3f r = As[i].block<3, 3>(0, 0).eulerAngles(0, 1, 2);
                Eigen::Vector3f t = As[i].block<3, 1>(0, 3);
                std::cout << "A" << i << ": ";
                std::cout << "Euler angles: " << r.transpose() << "\t";
                std::cout << "Translation: " << t.transpose() << std::endl;
                r = As[j].block<3, 3>(0, 0).eulerAngles(0, 1, 2);
                t = As[j].block<3, 1>(0, 3);
                std::cout << "A" << j << ": ";
                std::cout << "Euler angles: " << r.transpose() << "\t";
                std::cout << "Translation: " << t.transpose() << std::endl << std::endl;
#endif
                return true;
            }
        }
    }

    // control collision with workspace
    if (m_robot->getWorkspace() != nullptr) {
        shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>> workspace = m_robot->getWorkspace()->getFcl();

        if (checkFCL(workspace, base, m_zeroR, poseR, m_zeroT, poseT))
            return true;

        for (int i = 0; i < m_robot->getDim(); ++i) {
            // get R and t from A for first model
            if (checkFCL(workspace, models[i], m_zeroR, R[i], m_zeroT, t[i])) {
                Logging::debug("Collision between workspace and link " + std::to_string(i), this);
                return true;
            }
        }
    }
    return false;
}

bool CollisionDetection::checkPQP(shared_ptr<PQP_Model> &model1, shared_ptr<PQP_Model> &model2, Eigen::Matrix3f &R1,
                                  Eigen::Matrix3f &R2, Eigen::Vector3f &t1, Eigen::Vector3f &t2) {
    PQP_REAL pqpR1[3][3], pqpR2[3][3], pqpT1[3], pqpT2[3];

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            pqpR1[i][j] = R1(i, j);
            pqpR2[i][j] = R2(i, j);
        }
        pqpT1[i] = t1(i);
        pqpT2[i] = t2(i);
    }

    PQP_CollideResult cres;
    PQP_Collide(&cres, pqpR1, pqpT1, model1.get(), pqpR2, pqpT2, model2.get(), PQP_FIRST_CONTACT);
    if (cres.NumPairs() > 0)
        return true;
    else
        return false;
}

/*!
*  \brief      Check for collision with PQP or fcl library
*  \author     Sascha Kaden
*  \param[in]  MeshContainer one
*  \param[in]  MeshContainer two
*  \param[in]  rotation matrix one
*  \param[in]  rotation matrix two
*  \param[in]  translation vector one
*  \param[in]  translation vector two
*  \param[out] binary result of collision
*  \date       2016-07-14
*/
bool CollisionDetection::checkFCL(shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>> &model1,
                                  shared_ptr<fcl::BVHModel<fcl::OBBRSS<float>>> &model2, Eigen::Matrix3f &R1, Eigen::Matrix3f &R2,
                                  Eigen::Vector3f &t1, Eigen::Vector3f &t2) {
    o1 = new fcl::CollisionObject<float>(model1, R1, t1);
    o2 = new fcl::CollisionObject<float>(model2, R2, t2);
    fcl::CollisionRequest<float> request;    // default setting
    fcl::CollisionResult<float> result;
    collide(o1, o2, request, result);

    return result.isCollision();
}
