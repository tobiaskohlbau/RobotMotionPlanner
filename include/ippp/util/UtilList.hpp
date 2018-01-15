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

#ifndef UTILLIST_HPP
#define UTILLIST_HPP

#include <memory>
#include <string>

#include <ippp/dataObj/Node.hpp>

namespace ippp {
namespace util {

bool contains(const std::string &string, const std::string &subject);
void trimWhitespaces(std::string &str);
std::string getFileName(const std::string &filePath);

/*!
*  \brief      Removes Node by reference object
*  \author     Sascha Kaden
*  \param[in]  list
*  \param[in]  pointer of Node
*  \date       2016-12-19
*/
template <unsigned int dim>
void eraseFromList(std::vector<std::shared_ptr<Node<dim>>> &list, const std::shared_ptr<Node<dim>> &node) {
    list.erase(std::remove(list.begin(), list.end(), node), list.end());
}

/*!
*  \brief      Returns the Node with the lowest cost and removes it from the list
*  \author     Sascha Kaden
*  \param[in]  list
*  \param[out] Node with lowest cost
*  \date       2016-12-19
*/
template <unsigned int dim>
std::shared_ptr<Node<dim>> removeMinFromList(std::vector<std::shared_ptr<Node<dim>>> &list) {
    double min = std::numeric_limits<double>::max();
    std::shared_ptr<Node<dim>> minNode = nullptr;
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i]->getCost() < min) {
            minNode = list[i];
            min = list[i]->getCost();
        }
    }
    if (minNode != nullptr)
        eraseFromList(list, minNode);

    return minNode;
}

/*!
*  \brief      Return true, if the list contains the passed Node
*  \author     Sascha Kaden
*  \param[in]  list
*  \param[in]  Node
*  \param[out] true, if list contains passed Node
*  \date       2016-12-19
*/
template <unsigned int dim>
bool contains(const std::vector<std::shared_ptr<Node<dim>>> &list, const std::shared_ptr<Node<dim>> &node) {
    if (std::find(list.begin(), list.end(), node) != list.end())
        return true;

    return false;
}

} /* namespace util */
} /* namespace ippp */

#endif    // UTILLIST_HPP
