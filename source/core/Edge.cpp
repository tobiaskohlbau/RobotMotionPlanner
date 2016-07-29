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

#include <core/Edge.h>
#include <core/Node.h>

using std::shared_ptr;
using namespace rmpl;

/*!
*  \brief      Default constructor of the class Edge
*  \author     Sasch Kaden
*  \date       2016-05-30
*/
Edge::Edge()
    : Base("Edge") {
}

/*!
*  \brief      Constructor of the class Edge
*  \author     Sascha Kaden
*  \param[in]  source Node
*  \param[in]  target Node
*  \date       2016-05-25
*/
Edge::Edge(const shared_ptr<Node> &target, float length)
        : Base("Edge") {
    m_target = target;
    m_length = length;
}

/*!
*  \brief      Return legth of the Edge
*  \author     Sascha Kaden
*  \param[out] length of the Edge
*  \date       2016-05-25
*/
float Edge::getLength() {
    return m_length;
}

/*!
*  \brief      Set target Node of the Edge
*  \author     Sascha Kaden
*  \param[in]  target Node
*  \date       2016-05-25
*/
void Edge::setTarget(const shared_ptr<Node> &target, float length) {
    m_target = target;
}

/*!
*  \brief      Return target Node of the Edge
*  \author     Sascha Kaden
*  \param[out] target Node
*  \date       2016-05-25
*/
shared_ptr<Node> Edge::getTarget() {
    return m_target;
}