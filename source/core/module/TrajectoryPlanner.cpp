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

#include <core/module/TrajectoryPlanner.h>

#include <include/core/utility/Logging.h>

using namespace rmpl;
using std::shared_ptr;

/*!
*  \brief      Constructor of the class TrajectoryPlanner
*  \author     Sascha Kaden
*  \param[in]  TrajectoryMethod
*  \param[in]  pointer to ColllisionDetection instance
*  \date       2016-05-25
*/
TrajectoryPlanner::TrajectoryPlanner(float stepSize, const shared_ptr<CollisionDetection> &collision)
    : ModuleBase("TrajectoryPlanner") {
    m_collision = collision;

    setStepSize(stepSize);
}

/*!
*  \brief      Control the trajectory and return if possible or not
*  \author     Sascha Kaden
*  \param[in]  source Node
*  \param[in]  target Node
*  \param[out] possibility of trajectory, true if possible
*  \date       2016-05-31
*/
bool TrajectoryPlanner::controlTrajectory(const Node &source, const Node &target) {
    return controlTrajectory(source.getVec(), target.getVec());
}

/*!
*  \brief      Control the trajectory and return if possible or not
*  \author     Sascha Kaden
*  \param[in]  source Node
*  \param[in]  target Node
*  \param[out] possibility of trajectory, true if possible
*  \date       2016-05-31
*/
bool TrajectoryPlanner::controlTrajectory(const std::shared_ptr<Node> &source, const std::shared_ptr<Node> &target) {
    return controlTrajectory(source->getVec(), target->getVec());
}

/*!
*  \brief      Control the trajectory and return if possible or not
*  \author     Sascha Kaden
*  \param[in]  source Vec
*  \param[in]  target Vec
*  \param[out] possibility of trajectory, true if possible
*  \date       2016-05-31
*/
bool TrajectoryPlanner::controlTrajectory(const Vec<float> &source, const Vec<float> &target) {
    if (source.getDim() != target.getDim()) {
        Logging::error("Nodes/Vecs have different dimensions", this);
        return false;
    }

    std::vector<Vec<float>> path = computeTrajectory(source, target);
    if (m_collision->controlTrajectory(path))
        return false;

    return true;
}

/*!
*  \brief      Compute the trajectory and return the list of vecs
*  \author     Sascha Kaden
*  \param[in]  first Vec
*  \param[in]  second Vec
*  \param[out] trajectory
*  \date       2016-05-31
*/
std::vector<Vec<float>> TrajectoryPlanner::computeTrajectory(const Vec<float> &source, const Vec<float> &target) {
    std::vector<Vec<float>> vecs;
    if (source.getDim() != target.getDim()) {
        Logging::error("Vecs have different dimensions", this);
        return vecs;
    }

    Vec<float> u(target - source); // u = a - b
    u /= u.norm(); // u = |u|
    Vec<float> temp(source);
    while ((temp - target).sqNorm() > 1) {
        vecs.push_back(temp);
        temp += u * m_stepSize;
    }
    return vecs;
}

/*!
*  \brief      Compute the trajectory and return the list of vecs
*  \author     Sascha Kaden
*  \param[in]  first Vec
*  \param[in]  second Vec
*  \param[in]  step size
*  \param[out] trajectory
*  \date       2016-05-31
*/
std::vector<Vec<float>> TrajectoryPlanner::computeTrajectory(const Vec<float> &source, const Vec<float> &target, float stepSize) {
    std::vector<Vec<float>> vecs;
    if (source.getDim() != target.getDim()) {
        Logging::error("Vecs have different dimensions", this);
        return vecs;
    }

    if (stepSize <= 0) {
        stepSize = 1;
        Logging::warning("Step size has to be larger than 0, it has set to 1!", this);
    }

    // u = a - b    |   uNorm = |u|
    Vec<float> u(target - source); // u = a - b
    u /= u.norm(); // u = |u|
    Vec<float> temp(source);
    vecs.push_back(temp);
    while ((temp - target).sqNorm() > (15 * stepSize)) {
        temp += u * stepSize;
        vecs.push_back(temp);
    }
    return vecs;
}

/*!
*  \brief      Set step size, if it is larger than zero
*  \author     Sascha Kaden
*  \param[in]  step size
*  \date       2016-07-14
*/
void TrajectoryPlanner::setStepSize(float stepSize) {
    if (stepSize <= 0) {
        m_stepSize = 1;
        Logging::warning("Step size has to be larger than 0, it has set to 1!", this);
    } else {
        m_stepSize = stepSize;
    }
}

/*!
*  \brief      Return step size
*  \author     Sascha Kaden
*  \param[out] step size
*  \date       2016-07-14
*/
float TrajectoryPlanner::getStepSize() {
    return m_stepSize;
}