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

#ifndef HEURISTICINF_H
#define HEURISTICINF_H

#include <core/utility/heuristic/Heuristic.hpp>

namespace rmpl {

/*!
* \brief   Static class for the computation of heuristic costs from Edge
* \author  Sascha Kaden
* \date    2017-01-02
*/
template <unsigned int dim>
class HeuristicInf : public Heuristic<dim> {
  public:
    HeuristicInf();
    float calcEdgeCost(const Vector<dim> &source, const Vector<dim> &target) const override;
};

/*!
*  \brief      Standard constructor of the class HeuristicInf
*  \author     Sascha Kaden
*  \date       2017-02-19
*/
template <unsigned int dim>
HeuristicInf<dim>::HeuristicInf() : Heuristic<dim>("Heuristic Inf") {
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
float HeuristicInf<dim>::calcEdgeCost(const Vector<dim> &source, const Vector<dim> &target) const {
    return (source - target).maxCoeff();
}

} /* namespace rmpl */

#endif    // HEURISTICINF_H
