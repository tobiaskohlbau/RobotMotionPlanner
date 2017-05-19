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

#ifndef GAUSSIANSAMPLING_HPP
#define GAUSSIANSAMPLING_HPP

#include <core/sampling/Sampling.hpp>

namespace ippp {

/*!
* \brief   Class Sampling creates sample Vectors with the passed strategy, for creating single Vectors the Sampler is used.
* \author  Sascha Kaden
* \date    2016-12-20
*/
template <unsigned int dim>
class GaussianSampling : public Sampling<dim> {
  public:
    GaussianSampling(const std::shared_ptr<Environment> &environment, const std::shared_ptr<CollisionDetection<dim>> &collision,
                     const std::shared_ptr<TrajectoryPlanner<dim>> &trajectory, const std::shared_ptr<Sampler<dim>> &sampler,
                     const unsigned int attempts = 10, const float distance = 15);

    virtual Vector<dim> getSample();

  private:
    float m_distance;

    using Sampling<dim>::m_attempts;
    using Sampling<dim>::m_collision;
    using Sampling<dim>::m_sampler;
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
GaussianSampling<dim>::GaussianSampling(const std::shared_ptr<Environment> &environment,
                                        const std::shared_ptr<CollisionDetection<dim>> &collision,
                                        const std::shared_ptr<TrajectoryPlanner<dim>> &trajectory,
                                        const std::shared_ptr<Sampler<dim>> &sampler, const unsigned int attempts,
                                        const float distance)
    : Sampling<dim>(environment, collision, trajectory, sampler, attempts, "BridgeSampling"), m_distance(distance) {
}

/*!
*  \brief      Return sample
*  \author     Sascha Kaden
*  \param[out] sample Vec
*  \date       2016-12-20
*/
template <unsigned int dim>
Vector<dim> GaussianSampling<dim>::getSample() {
    unsigned int count = 0;
    Vector<dim> sample1, sample2, ray;

    while (true) {
        if (count > m_attempts)
            return util::NaNVector<dim>();
        else
            ++count;

        sample1 = m_sampler->getSample();
        ray = m_sampler->getRandomRay();
        ray *= m_distance * m_sampler->getRandomNumber();
        sample2 = sample1 + ray;

        if (!m_collision->controlVec(sample1) && m_collision->controlVec(sample2))
            return sample1;
        else if (m_collision->controlVec(sample1) && !m_collision->controlVec(sample2))
            return sample2;
    }
    return util::NaNVector<dim>();
}

} /* namespace ippp */

#endif /* GAUSSIANSAMPLING_HPP */
