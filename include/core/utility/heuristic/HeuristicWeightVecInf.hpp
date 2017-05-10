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

#ifndef HEURISTICWEIGHTVECINF_H
#define HEURISTICWEIGHTVECINF_H

#include <core/utility/heuristic/Heuristic.hpp>

namespace ippp {

/*!
* \brief   Static class for the computation of heuristic costs from Edge
* \author  Sascha Kaden
* \date    2017-01-02
*/
template <unsigned int dim>
class HeuristicWeightVecInf : public Heuristic<dim> {
  public:
    HeuristicWeightVecInf();
    HeuristicWeightVecInf(const Vector<dim> &weightVec);
    float calcEdgeCost(const Vector<dim> &source, const Vector<dim> &target) const override;

    void setWeightVec(const Vector<dim> &vec);
    Vector<dim> getWeightVec() const;

  private:
    Vector<dim> m_weightVec;
};

/*!
*  \brief      Standard constructor of the class HeuristicWeightVecInf
*  \author     Sascha Kaden
*  \date       2017-02-19
*/
template <unsigned int dim>
HeuristicWeightVecInf<dim>::HeuristicWeightVecInf() : Heuristic<dim>("Heuristic weightVecInf") {
}

/*!
*  \brief      Standard constructor of the class HeuristicWeightVecInf
*  \author     Sascha Kaden
*  \param[in]  weightVec
*  \date       2017-02-19
*/
template <unsigned int dim>
HeuristicWeightVecInf<dim>::HeuristicWeightVecInf(const Vector<dim> &weightVec) : Heuristic<dim>("Heuristic weightVecInf") {
    setWeightVec(weightVec);
}

/*!
*  \brief      Calculates the heuristic cost of an Edge from the source and target Node by the specified heuristic.
*  \author     Sascha Kaden
*  \param[in]  source Node
*  \param[in]  target Node
*  \param[out] heuristic cost
*  \date       2017-01-02
*/
template <unsigned int dim>
float HeuristicWeightVecInf<dim>::calcEdgeCost(const Vector<dim> &source, const Vector<dim> &target) const {
    return (source - target).cwiseProduct(m_weightVec).maxCoeff();
}

/*!
*  \brief      Sets the weighting Vector
*  \author     Sascha Kaden
*  \param[in]  Vector
*  \date       2017-01-02
*/
template <unsigned int dim>
void HeuristicWeightVecInf<dim>::setWeightVec(const Vector<dim> &vec) {
    m_weightVec = vec;
}

/*!
*  \brief      Returns the weighting Vector
*  \author     Sascha Kaden
*  \param[in]  Vector
*  \date       2017-01-02
*/
template <unsigned int dim>
Vector<dim> HeuristicWeightVecInf<dim>::getWeightVec() const {
    return m_weightVec;
}

} /* namespace ippp */

#endif    // HEURISTICWEIGHTVECINF_H
