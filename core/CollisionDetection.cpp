#include <core/CollisionDetection.h>

using namespace rmpl;

/*!
*  \brief      Constructor of the class CollisionDetection
*  \author     Sascha Kaden
*  \param[in]  VREP Helper
*  \param[in]  RobotType
*  \date       2016-06-30
*/
CollisionDetection::CollisionDetection(const std::shared_ptr<Helper> &vrep, const std::shared_ptr<RobotBase> &robot)
        : Base("CollisionDetection") {
    m_robot = robot;
    m_vrep = vrep;
    m_zero(0,0) = 0;
}

/*!
*  \brief      Check for collision
*  \author     Sascha Kaden
*  \param[in]  vec
*  \param[out] possibility of collision
*  \date       2016-05-25
*/
bool CollisionDetection::controlCollision(const Vec<float> &vec) {
    assert(vec.getDim() == m_robot->getDim());

    switch (m_robot->getType()) {
        case RobotType::POINT_ROBOT:
            return controlCollisionPointRobot(vec[0], vec[1]);
            break;
        case RobotType::JACO:
            return controlCollisionVrep(vec);
            break;
        default:
            return false;
    }
}

/*!
*  \brief      Check for collision
*  \author     Sascha Kaden
*  \param[in]  vector of points
*  \param[out] possibility of collision
*  \date       2016-05-25
*/
bool CollisionDetection::controlCollision(const std::vector<Vec<float>> &vecs) {
    if (vecs.size() == 0)
        return false;

    assert(vecs[0].getDim() == m_robot->getDim());

    switch (m_robot->getType()) {
        case RobotType::POINT_ROBOT:
            for (int i = 0; i < vecs.size(); ++i)
                if (controlCollisionPointRobot(vecs[i][0], vecs[i][1]))
                    return true;
            break;
        case RobotType::JACO:
            return controlCollisionVrep(vecs);
            break;
        default:
            return false;
    }
}

/*!
*  \brief      Check for 2D PointRobot collision
*  \author     Sascha Kaden
*  \param[in]  x
*  \param[in]  y
*  \param[out] possibility of collision, true if in collision
*  \date       2016-06-30
*/
bool CollisionDetection::controlCollisionPointRobot(const float &x, const float &y) {
    if (m_workspace.rows() == -1 && m_workspace.cols() == -1) {
        this->sendMessage("Empty workspace!");
        return false;
    }

    if (m_workspace(x,y) < 30) {
        return true;
    }
    else {
        return false;
    }
    return false;
}

/*!
*  \brief      Check for vrep collision
*  \author     Sascha Kaden
*  \param[in]  vec
*  \param[out] possibility of collision
*  \date       2016-06-30
*/
bool CollisionDetection::controlCollisionVrep(const Vec<float> &vec) {
    assert(vec.getDim() == 6);
    return m_vrep->checkCollision(vec);
}

/*!
*  \brief      Check for vrep collision
*  \author     Sascha Kaden
*  \param[in]  vector of vec
*  \param[out] possibility of collision
*  \date       2016-06-30
*/
bool CollisionDetection::controlCollisionVrep(const std::vector<Vec<float>> &vecs) {
    assert(vecs[0].getDim() == 6);
    return m_vrep->checkCollision(vecs);
}

/*!
*  \brief      Set workspace for 2D collision check
*  \author     Sascha Kaden
*  \param[in]  workspace
*  \date       2016-05-25
*/
void CollisionDetection::set2DWorkspace(Eigen::MatrixXi space) {
    if (this->m_workspace.rows() == -1 || this->m_workspace.cols() == -1) {
        this->sendMessage("Empty space given!");
        return;
    }

    m_workspace = space;
}

/*!
*  \brief      Return workspace for 2D collision
*  \author     Sascha Kaden
*  \param[out] workspace
*  \date       2016-05-25
*/
Eigen::MatrixXi CollisionDetection::get2DWorkspace() const {
    return m_workspace;
}
