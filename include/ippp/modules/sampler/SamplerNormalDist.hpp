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

#ifndef SAMPLERNORMALDIST_HPP
#define SAMPLERNORMALDIST_HPP

#include <ippp/modules/sampler/Sampler.hpp>

namespace ippp {

/*!
* \brief   Class SamplerNormalDist create samples by a normal distribution.
* \author  Sascha Kaden
* \date    2016-05-23
*/
template <unsigned int dim>
class SamplerNormalDist : public Sampler<dim> {
  public:
    SamplerNormalDist(const std::shared_ptr<Environment> &environment, const std::string &seed = "");
    Vector<dim> getSample() override;
    void setOrigin(const Vector<dim> &origin) override;

  private:
    std::vector<std::normal_distribution<double>> m_distNormal;
};

/*!
*  \brief      Constructor of the class SamplerNormalDist
*  \author     Sascha Kaden
*  \param[in]  Environment
*  \param[in]  seed
*  \date       2016-05-24
*/
template <unsigned int dim>
SamplerNormalDist<dim>::SamplerNormalDist(const std::shared_ptr<Environment> &environment, const std::string &seed)
    : Sampler<dim>("SamplerNormalDist", environment, seed) {
    m_distNormal.clear();
    for (unsigned int i = 0; i < dim; ++i) {
        std::normal_distribution<double> distribution(this->m_origin[i], this->m_maxBoundary[i] - this->m_minBoundary[i]);
        m_distNormal.push_back(distribution);
    }
}

/*!
*  \brief      Return sample by a normal distribution
*  \author     Sascha Kaden
*  \param[out] sample
*  \date       2016-05-24
*/
template <unsigned int dim>
Vector<dim> SamplerNormalDist<dim>::getSample() {
    Vector<dim> config;
    double number;
    for (unsigned int i = 0; i < dim; ++i) {
        do {
            number = m_distNormal[i](this->m_generator);
        } while ((number <= this->m_minBoundary[i]) || (number >= this->m_maxBoundary[i]));
        config[i] = number;
    }
    return config;
}

/*!
*  \brief      Set the origin of the normal distribution
*  \author     Sascha Kaden
*  \param[in]  origin
*  \date       2016-11-14
*/
template <unsigned int dim>
void SamplerNormalDist<dim>::setOrigin(const Vector<dim> &origin) {
    m_distNormal.clear();
    for (unsigned int i = 0; i < dim; ++i) {
        std::normal_distribution<double> distribution(origin[i], this->m_maxBoundary[i] - this->m_minBoundary[i]);
        m_distNormal.push_back(distribution);
    }
}

} /* namespace ippp */

#endif /* SAMPLERNORMALDIST_HPP */
