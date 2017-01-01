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

#ifndef HEURISTIC_H
#define HEURISTIC_H

#include <core/dataObj/Node.hpp>

namespace rmpl {

template <unsigned int dim>
class Node;

enum EdgeHeuristic { euclidean, weightedVec, A, B, C};
enum NodeHeuristic { norm};

template <unsigned int dim>
class Heuristic {
public:
    static float calcEdgeCost(const std::shared_ptr<Node<dim>> &source, const std::shared_ptr<Node<dim>> &target);
    static float calcNodeCost(const std::shared_ptr<Node<dim>> &node);

    static void setEdgeHeuristic(EdgeHeuristic heuristic);
    static void setNodeHeuristic(NodeHeuristic heuristic);
    static EdgeHeuristic getEdgeHeuristic();
    static NodeHeuristic getNodeHeuristic();

private:
    static EdgeHeuristic m_edgeHeuristic;
    static NodeHeuristic m_nodeHeuristic;
};

template <unsigned int dim>
EdgeHeuristic Heuristic<dim>::m_edgeHeuristic = EdgeHeuristic::euclidean;
template <unsigned int dim>
NodeHeuristic Heuristic<dim>::m_nodeHeuristic = NodeHeuristic::norm;

template <unsigned int dim>
float Heuristic<dim>::calcEdgeCost(const std::shared_ptr<Node<dim>> &source,
                                   const std::shared_ptr<Node<dim>> &target) {
    return (source->getValues() - target->getValues()).norm();
}

template <unsigned int dim>
float Heuristic<dim>::calcNodeCost(const std::shared_ptr<Node<dim>> &source) {
    return source->getValues().norm();
}

template <unsigned int dim>
void Heuristic<dim>::setEdgeHeuristic(EdgeHeuristic heuristic) {
    m_edgeHeuristic = heuristic;
}

template <unsigned int dim>
void Heuristic<dim>::setNodeHeuristic(NodeHeuristic heuristic) {
    m_nodeHeuristic = heuristic;
}

template <unsigned int dim>
EdgeHeuristic Heuristic<dim>::getEdgeHeuristic() {
    return m_edgeHeuristic;
}

template <unsigned int dim>
NodeHeuristic Heuristic<dim>::getNodeHeuristic() {
    return m_nodeHeuristic;
}

} /* namespace rmpl */

#endif // HEURISTIC_H