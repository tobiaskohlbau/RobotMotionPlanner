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

#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include <math.h>

#include <Eigen/Core>

#include <core/Identifier.h>
#include <core/collisionDetection/CollisionDetection.hpp>
#include <core/sampler/Sampler.hpp>
#include <core/trajectoryPlanner/TrajectoryPlanner.hpp>
#include <environment/Environment.h>

namespace ippp {

/*!
* \brief   Class Sampling creates sample Vectors with the passed strategy, for creating single Vectors the Sampler is used.
* \author  Sascha Kaden
* \date    2016-12-20
*/
template <unsigned int dim>
class Sampling : public Identifier {
  public:
    Sampling(const std::shared_ptr<Environment> &environment, const std::shared_ptr<CollisionDetection<dim>> &collision,
             const std::shared_ptr<TrajectoryPlanner<dim>> &trajectory, const std::shared_ptr<Sampler<dim>> &sampler,
             const unsigned int attempts = 10, const std::string &name = "Sampling");

    virtual Vector<dim> getSample();
    double getRandomNumber() const;
    void setOrigin(const Vector<dim> &origin);

  protected:
    const unsigned int m_attempts;

    std::shared_ptr<CollisionDetection<dim>> m_collision = nullptr;
    std::shared_ptr<Sampler<dim>> m_sampler = nullptr;
    std::shared_ptr<TrajectoryPlanner<dim>> m_trajectory = nullptr;
};

/*!
*  \brief      Constructor of the class Sampling
*  \author     Sascha Kaden
*  \param[in]  robot
*  \param[in]  CollisionDetection
*  \param[in]  TrajectoryPlanner
*  \param[in]  Sampler
*  \date       2016-12-20
*/
template <unsigned int dim>
Sampling<dim>::Sampling(const std::shared_ptr<Environment> &environment,
                        const std::shared_ptr<CollisionDetection<dim>> &collision,
                        const std::shared_ptr<TrajectoryPlanner<dim>> &trajectory, const std::shared_ptr<Sampler<dim>> &sampler,
                        const unsigned int attempts, const std::string &name)
    : Identifier(name), m_attempts(attempts) {
    m_collision = collision;
    m_trajectory = trajectory;
    m_sampler = sampler;
}

/*!
*  \brief      Return sample
*  \author     Sascha Kaden
*  \param[out] sample Vec
*  \date       2016-12-20
*/
template <unsigned int dim>
Vector<dim> Sampling<dim>::getSample() {
    return m_sampler->getSample();
}

/*!
*  \brief      Return random number between 0 and 1
*  \author     Sascha Kaden
*  \param[out] random number
*  \date       2017-04-03
*/
template <unsigned int dim>
double Sampling<dim>::getRandomNumber() const {
    return m_sampler->getRandomNumber();
}

/*!
*  \brief      Set the origin of the Sampler
*  \author     Sascha Kaden
*  \param[in]  origin
*  \date       2016-12-20
*/
template <unsigned int dim>
void Sampling<dim>::setOrigin(const Vector<dim> &origin) {
    m_sampler->setOrigin(origin);
}

} /* namespace ippp */

#endif /* SAMPLING_HPP */
