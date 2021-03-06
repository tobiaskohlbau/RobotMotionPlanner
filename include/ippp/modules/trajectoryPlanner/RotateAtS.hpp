//-------------------------------------------------------------------------//
//
// Copyright 2017 Sascha Kaden
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

#ifndef ROTATEATS_HPP
#define ROTATEATS_HPP

#include <ippp/modules/trajectoryPlanner/TrajectoryPlanner.hpp>

namespace ippp {

/*!
* \brief   Class LinearTrajectory plans a path between the passed nodes/configs. Start and end point aren't part of the path.
* \details The rotation in the path will be at the rotation point, percent value of the value.
 * The values of the rotation will be taken from the robot, which is saved in the environment.
* \author  Sascha Kaden
* \date    2016-05-25
*/
template <unsigned int dim>
class RotateAtS : public TrajectoryPlanner<dim> {
  public:
    RotateAtS(const std::shared_ptr<CollisionDetection<dim>> &collision,
              const std::shared_ptr<Environment> &environment, const double posRes = 1, const double oriRes = 0.1, const double rotPoint = 0.5);

    std::vector<Vector<dim>> calcTrajectoryCont(const Vector<dim> &source, const Vector<dim> &target);
    std::vector<Vector<dim>> calcTrajectoryBin(const Vector<dim> &source, const Vector<dim> &target);

    void setRotationPoint(const double rotPoint);
    double getRotationPoint();

  private:
    void initMasks();

    double m_rotationPoint = 0.5;

    using TrajectoryPlanner<dim>::m_collision;
    using TrajectoryPlanner<dim>::m_environment;
    using TrajectoryPlanner<dim>::m_posRes;
    using TrajectoryPlanner<dim>::m_oriRes;
    using TrajectoryPlanner<dim>::m_sqPosRes;
    using TrajectoryPlanner<dim>::m_sqOriRes;
    using TrajectoryPlanner<dim>::m_posMask;
    using TrajectoryPlanner<dim>::m_oriMask;
};

/*!
*  \brief      Constructor of the class LinearTrajectory
*  \author     Sascha Kaden
*  \param[in]  CollisionDetection
*  \param[in]  Environment
*  \param[in]  position resolution
*  \param[in]  orientation resolution
*  \param[in]  rotation point
*  \date       2017-06-20
*/
template <unsigned int dim>
RotateAtS<dim>::RotateAtS(const std::shared_ptr<CollisionDetection<dim>> &collision,
                          const std::shared_ptr<Environment> &environment, const double posRes, const double oriRes, const double rotPoint)
    : TrajectoryPlanner<dim>("RotateAtS", collision, environment, posRes, oriRes) {
    setRotationPoint(rotPoint);

}

/*!
*  \brief      Compute the binary (section wise) trajectory between source and target. Return vector of points.
*  \details    The trajectory doesn't contain source or target vector.
*  \author     Sascha Kaden
*  \param[in]  source Vector
*  \param[in]  target Vector
*  \param[out] trajectory
*  \date       2017-06-20
*/
template <unsigned int dim>
std::vector<Vector<dim>> RotateAtS<dim>::calcTrajectoryBin(const Vector<dim> &source, const Vector<dim> &target) {
    // Todo: implementation of binary rotate at s trajectory
    return calcTrajectoryCont(source, target);
}

/*!
*  \brief      Compute the continuous trajectory between source and target. Return vector of points.
*  \details    The trajectory doesn't contain source or target vector.
*  \author     Sascha Kaden
*  \param[in]  source Vector
*  \param[in]  target Vector
*  \param[out] trajectory
*  \date       2017-06-20
*/
template <unsigned int dim>
std::vector<Vector<dim>> RotateAtS<dim>::calcTrajectoryCont(const Vector<dim> &source, const Vector<dim> &target) {
    std::vector<Vector<dim>> configs;

    Vector<dim> posSource = util::multiplyElementWise<dim>(source, m_posMask);
    Vector<dim> posTarget = util::multiplyElementWise<dim>(target, m_posMask);

    // compute the linear translation points
    configs = util::linearTrajectoryCont<dim>(posSource, posTarget, m_posRes, m_oriRes, m_posMask, m_oriMask);
    Vector<dim> u(posTarget - posSource);

    // compute the linear rotation points
    Vector<dim> rotSource = util::multiplyElementWise<dim>(source, m_oriMask);
    Vector<dim> rotTarget = util::multiplyElementWise<dim>(target, m_oriMask);
    std::vector<Vector<dim>> oriConfigs = util::linearTrajectoryCont<dim>(rotSource, rotTarget, m_posRes, m_oriRes, m_posMask, m_oriMask);

    // get middle point and insert rotation configurations
    size_t middleIndex = static_cast<size_t>(configs.size() * m_rotationPoint);
    configs.insert(std::begin(configs) + middleIndex, std::begin(oriConfigs), std::end(oriConfigs));

    return configs;
}

/*!
*  \brief      Sets to rotation point and checks if it is inside of the boundings.
*  \author     Sascha Kaden
*  \param[in]  rotation point
*  \date       2017-06-20
*/
template <unsigned int dim>
void RotateAtS<dim>::setRotationPoint(const double rotPoint) {
    if (rotPoint > 0 && 1 > rotPoint) {
        m_rotationPoint = rotPoint;
    } else {
        m_rotationPoint = 0.5;
        Logging::error("Rotation point have to be between 0 and 1!", this);
    }
}

/*!
*  \brief      Returns to rotation point.
*  \author     Sascha Kaden
*  \param[out] rotation point
*  \date       2017-06-20
*/
template <unsigned int dim>
double RotateAtS<dim>::getRotationPoint() {
    return m_rotationPoint;
}

} /* namespace ippp */

#endif /* ROTATEATS_HPP */
