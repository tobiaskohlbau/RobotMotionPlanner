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

#ifndef UTILVEC_H
#define UTILVEC_H

#include <Eigen/Core>

#include <core/types.h>

namespace rmpl {
namespace utilVec {

/*!
*  \brief      Appends the second Vector to the first one and returns the result Vector.
*  \author     Sascha Kaden
*  \param[in]  first Vector
*  \param[in]  second Vector
*  \param[out] result Vector
*  \date       2016-12-23
*/
template <unsigned int dim1, unsigned int dim2>
Vector<dim1 + dim2> append(Vector<dim1> first, Vector<dim2> second) {
    Vector<dim1 + dim2> vec;
    for (int i = 0; i < dim1; ++i)
        vec[i] = first[i];
    for (int i = 0; i < dim2; ++i)
        vec[dim1 + i] = second[i];
    return vec;
}

/*!
*  \brief      Appends the value to the first one and returns the result Vector.
*  \author     Sascha Kaden
*  \param[in]  first Vector
*  \param[in]  value
*  \param[out] result Vector
*  \date       2016-12-23
*/
template <unsigned int dim>
Vector<dim + 1> append(Vector<dim> source, float add) {
    Vector<dim + 1> vec;
    for (int i = 0; i < dim; ++i)
        vec[i] = source[i];
    vec[dim] = add;
    return vec;
}

/*!
*  \brief      Creates a 5 dimensional Vector with passed values
*  \author     Sascha Kaden
*  \param[in]  x
*  \param[in]  y
*  \param[in]  z
*  \param[in]  rx
*  \param[in]  ry
*  \param[out] result Vector
*  \date       2016-12-23
*/
static Vector5 Vecf(float x, float y, float z, float rx, float ry) {
    Vector5 vec;
    vec << x, y, z, rx, ry;
    return vec;
}

/*!
*  \brief      Creates a 6 dimensional Vector with passed values
*  \author     Sascha Kaden
*  \param[in]  x
*  \param[in]  y
*  \param[in]  z
*  \param[in]  rx
*  \param[in]  ry
*  \param[in]  rz
*  \param[out] result Vector
*  \date       2016-12-23
*/
static Vector6 Vecf(float x, float y, float z, float rx, float ry, float rz) {
    Vector6 vec;
    vec << x, y, z, rx, ry, rz;
    return vec;
}

/*!
*  \brief      Creates a Vector with template dimension and all elements are set to the passed value
*  \author     Sascha Kaden
*  \param[in]  value
*  \param[out] result Vector
*  \date       2016-12-23
*/
template <unsigned int dim>
Vector<dim> Vecf(float data) {
    Vector<dim> vec;
    for (unsigned int i = 0; i < dim; ++i)
        vec[i] = data;
    return vec;
}

/*!
*  \brief      Creates a Vector with passed array (dimension by template parameter)
*  \author     Sascha Kaden
*  \param[in]  array
*  \param[out] result Vector
*  \date       2016-12-23
*/
template <unsigned int dim>
Vector<dim> Vecf(float data[]) {
    Vector<dim> vec;
    for (unsigned int i = 0; i < dim; ++i)
        vec[i] = data[i];
    return vec;
}

/*!
*  \brief      Creates a Vector with passed dimension and array
*  \author     Sascha Kaden
*  \param[in]  dim
*  \param[in]  array
*  \param[out] result Vector
*  \date       2016-12-23
*/
static VectorX Vecf(unsigned int dim, float data[]) {
    VectorX vec(dim);
    for (unsigned int i = 0; i < dim; ++i)
        vec[i] = data[i];
    return vec;
}

} /* namespace utilVec */
} /* namespace rmpl */

#endif    // UTILVEC_H
