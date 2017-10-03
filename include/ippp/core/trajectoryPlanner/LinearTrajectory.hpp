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

#ifndef LINEARTRAJECTORY_HPP
#define LINEARTRAJECTORY_HPP

#include <ippp/core/trajectoryPlanner/TrajectoryPlanner.hpp>

namespace ippp {

/*!
* \brief   Class LinearTrajectory plans a linear path between the passed nodes/vecs. Start and end point aren't part of the path.
* \author  Sascha Kaden
* \date    2016-05-25
*/
template <unsigned int dim>
class LinearTrajectory : public TrajectoryPlanner<dim> {
  public:
    LinearTrajectory(const std::shared_ptr<CollisionDetection<dim>> &collision, const std::shared_ptr<Environment> &environment,
                     const double stepSize = 1);

    std::vector<Vector<dim>> calcTrajectoryCont(const Vector<dim> &source, const Vector<dim> &target);
    std::vector<Vector<dim>> calcTrajectoryBin(const Vector<dim> &source, const Vector<dim> &target);

  private:
    using TrajectoryPlanner<dim>::m_collision;
    using TrajectoryPlanner<dim>::m_stepSize;
    using TrajectoryPlanner<dim>::m_sqStepSize;
};

/*!
*  \brief      Constructor of the class LinearTrajectory
*  \author     Sascha Kaden
*  \param[in]  CollisionDetection
*  \param[in]  Environment
*  \param[in]  step size of the path
*  \date       2016-05-25
*/
template <unsigned int dim>
LinearTrajectory<dim>::LinearTrajectory(const std::shared_ptr<CollisionDetection<dim>> &collision,
                                        const std::shared_ptr<Environment> &environment, const double stepSize)
    : TrajectoryPlanner<dim>("LinearTrajectory", collision, environment, stepSize) {
}

/*!
*  \brief      Compute the binary (section wise) trajectory between source and target. Return vector of points.
*  \details    The trajectory doesn't contain source or target vector.
*  \author     Sascha Kaden
*  \param[in]  source Vector
*  \param[in]  target Vector
*  \param[out] trajectory
*  \date       2016-12-21
*/
template <unsigned int dim>
std::vector<Vector<dim>> LinearTrajectory<dim>::calcTrajectoryBin(const Vector<dim> &source, const Vector<dim> &target) {
    return util::linearTrajectoryBin<dim>(source, target, m_stepSize);
}

/*!
*  \brief      Compute the continuous trajectory between source and target. Return vector of points.
*  \details    The trajectory doesn't contain source or target vector.
*  \author     Sascha Kaden
*  \param[in]  source Vector
*  \param[in]  target Vector
*  \param[out] trajectory
*  \date       2016-12-21
*/
template <unsigned int dim>
std::vector<Vector<dim>> LinearTrajectory<dim>::calcTrajectoryCont(const Vector<dim> &source, const Vector<dim> &target) {
    return util::linearTrajectoryCont<dim>(source, target, m_stepSize);
}

} /* namespace ippp */

#endif /* LINEARTRAJECTORY_HPP */
