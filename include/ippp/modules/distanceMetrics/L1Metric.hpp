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

#ifndef L1METRIC_HPP
#define L1METRIC_HPP

#include <ippp/modules/distanceMetrics/DistanceMetric.hpp>

namespace ippp {

/*!
* \brief   L1Metric computes the distance of nodes/configurations with the L1 metric.
* \author  Sascha Kaden
* \date    2017-01-02
*/
template <unsigned int dim>
class L1Metric : public DistanceMetric<dim> {
  public:
    L1Metric();
    double calcDist(const Vector<dim> &source, const Vector<dim> &target) const;
    double calcSimpleDist(const Vector<dim> &source, const Vector<dim> &target) const;
    void simplifyDist(double &dist) const;
};

/*!
*  \brief      Standard constructor of the class L1Metric.
*  \author     Sascha Kaden
*  \date       2017-02-19
*/
template <unsigned int dim>
L1Metric<dim>::L1Metric() : DistanceMetric<dim>("L1 Metric") {
}

/*!
*  \brief      Calculates the distance cost of an Edge from the source and target Node by the L1 metric.
*  \author     Sascha Kaden
*  \param[in]  source vector
*  \param[in]  target vector
*  \param[out] distance cost
*  \date       2017-01-02
*/
template <unsigned int dim>
double L1Metric<dim>::calcDist(const Vector<dim> &source, const Vector<dim> &target) const {
    return (source - target).sum();
}

/*!
*  \brief      Calculates the squared distance cost of an Edge from the source and target Node by the L1 metric.
*  \author     Sascha Kaden
*  \param[in]  source vector
*  \param[in]  target vector
*  \param[out] distance cost
*  \date       2017-10-08
*/
template <unsigned int dim>
double L1Metric<dim>::calcSimpleDist(const Vector<dim> &source, const Vector<dim> &target) const {
    return (source - target).sum();
}

/*!
*  \brief      Calculates the simplified distance of the passed distance.
*  \author     Sascha Kaden
*  \param[in]  distance
*  \param[out] simplified distance
*  \date       2017-10-08
*/
template <unsigned int dim>
void L1Metric<dim>::simplifyDist(double &dist) const {
}

} /* namespace ippp */

#endif    // L1METRIC_HPP
