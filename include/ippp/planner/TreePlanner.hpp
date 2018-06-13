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

#ifndef TREETPLANNER_HPP
#define TREETPLANNER_HPP

#include <mutex>

#include <ippp/planner/Planner.hpp>
#include <ippp/planner/options/RRTOptions.hpp>

namespace ippp {

/*!
* \brief   Super class of all tree path planners like RRT and EST.
* \author  Sascha Kaden
* \date    2017-06-20
*/
template <unsigned int dim>
class TreePlanner : public Planner<dim> {
  public:
    TreePlanner(const std::string &name, const std::shared_ptr<Environment> &environment, const PlannerOptions<dim> &options,
                const std::shared_ptr<Graph<dim>> &graph);

    virtual bool computePath(const Vector<dim> start, const Vector<dim> goal, size_t numNodes, size_t numThreads = 1);
    virtual bool computePath(const Vector<dim> start, const std::vector<Vector<dim>> goals, size_t numNodes,
                             size_t numThreads = 1);
    virtual bool computePathToPose(const Vector<dim> startConfig, const Vector6 goalPose, const std::pair<Vector6, Vector6> &C,
                                   size_t numNodes, size_t numThreads = 1);
    virtual bool computePathToPose(const Vector<dim> startConfig, const std::vector<Vector6> pathPoses,
                                   const std::pair<Vector6, Vector6> &C, size_t numNodes, size_t numThreads = 1);

    virtual bool setInitNode(const Vector<dim> start);
    virtual bool connectGoalNode(const Vector<dim> goal) = 0;

    std::vector<std::shared_ptr<Node<dim>>> getPathNodes();
    std::vector<Vector<dim>> getPath(double posRes = 1, double oriRes = 0.1);
    std::shared_ptr<Node<dim>> getInitNode() const;
    std::shared_ptr<Node<dim>> getGoalNode() const;

  protected:
    std::shared_ptr<Node<dim>> m_initNode = nullptr;
    std::shared_ptr<Node<dim>> m_goalNode = nullptr;

    using Planner<dim>::m_validityChecker;
    using Planner<dim>::m_environment;
    using Planner<dim>::m_evaluator;
    using Planner<dim>::m_graph;
    using Planner<dim>::m_options;
    using Planner<dim>::m_pathPlanned;
    using Planner<dim>::m_sampling;
    using Planner<dim>::m_trajectory;
    using Planner<dim>::updateStats;
    using Planner<dim>::m_plannerCollector;
};

/*!
*  \brief      Constructor of the class TreePlanner, base class of RRT and EST.
*  \author     Sascha Kaden
*  \param[in]  name
*  \param[in]  Environment
*  \param[in]  PlannerOptions
*  \param[in]  Graph
*  \date       2017-06-20
*/
template <unsigned int dim>
TreePlanner<dim>::TreePlanner(const std::string &name, const std::shared_ptr<Environment> &environment,
                              const PlannerOptions<dim> &options, const std::shared_ptr<Graph<dim>> &graph)
    : Planner<dim>(name, environment, options, graph) {
}

/*!
*  \brief      Compute path from start Node<dim> to goal Node<dim> with passed number of samples and threads
*  \author     Sascha Kaden
*  \param[in]  start configuration
*  \param[in]  goal configuration
*  \param[in]  number of samples
*  \param[in]  number of threads
*  \param[out] true, if path was found
*  \date       2017-06-20
*/
template <unsigned int dim>
bool TreePlanner<dim>::computePath(const Vector<dim> start, const Vector<dim> goal, size_t numNodes, size_t numThreads) {
    m_plannerCollector->startPlannerTimer();
    this->initParams(start, goal);
    if (!setInitNode(start))
        return false;

    if (!m_validityChecker->check(goal)) {
        Logging::error("Goal configuration is not valid", this);
        return false;
    }

    m_evaluator->setConfigs(std::vector<Vector<dim>>{goal});

    size_t loopCount = 1;
    while (!m_evaluator->evaluate()) {
        Logging::info("Iteration: " + std::to_string(loopCount++), this);
        this->expand(numNodes, numThreads);
    }

    updateStats();
    m_plannerCollector->stopPlannerTimer();
    return connectGoalNode(goal);
}

template <unsigned int dim>
bool TreePlanner<dim>::computePath(const Vector<dim> start, const std::vector<Vector<dim>> goals, size_t numNodes,
                                   size_t numThreads) {
    Logging::warning("Path computation with multiple configuration goals is not implemented at the time", this);
    return false;
}

/*!
*  \brief      Compute path from start Node<dim> to goal Node<dim> with passed number of samples and threads
*  \author     Sascha Kaden
*  \param[in]  start Node
*  \param[in]  goal Node
*  \param[in]  number of samples
*  \param[in]  number of threads
*  \param[out] true, if path was found
*  \date       2017-06-20
*/
template <unsigned int dim>
bool TreePlanner<dim>::computePathToPose(const Vector<dim> startConfig, const Vector6 goalPose,
                                         const std::pair<Vector6, Vector6> &C, size_t numNodes, size_t numThreads) {
    this->initParams(startConfig, util::NaNVector<dim>());
    m_plannerCollector->startPlannerTimer();
    if (!setInitNode(startConfig))
        return false;

    m_evaluator->setPoses(std::vector<Vector6>{goalPose});

    size_t loopCount = 1;
    while (!m_evaluator->evaluate()) {
        Logging::info("Iteration: " + std::to_string(loopCount++), this);
        this->expand(numNodes, numThreads);
    }

    // set goal node
    double minDisplacement = std::numeric_limits<double>::max();
    auto robot = m_environment->getRobot();
    for (auto &node : m_graph->getNodes()) {
        if (util::checkConfigToPose<dim>(node->getValues(), goalPose, *robot, C)) {
            double displacement = util::calcConfigToPose<dim>(node->getValues(), goalPose, *robot).squaredNorm();
            if (displacement < minDisplacement) {
                minDisplacement = displacement;
                m_goalNode = node;
                m_pathPlanned = true;
            }
        }
    }

    m_plannerCollector->stopPlannerTimer();
    updateStats();
    return m_pathPlanned;
}

template <unsigned int dim>
bool TreePlanner<dim>::computePathToPose(const Vector<dim> startConfig, const std::vector<Vector6> pathPoses,
                                         const std::pair<Vector6, Vector6> &C, size_t numNodes, size_t numThreads) {
    Logging::warning("Path computation with multiple pose goals is not implemented at the time", this);
    return false;
}

/*!
*  \brief      Set init Node of the TreePlanner
*  \author     Sascha Kaden
*  \param[in]  initial Node
*  \param[out] true, if valid
*  \date       2017-06-20
*/
template <unsigned int dim>
bool TreePlanner<dim>::setInitNode(const Vector<dim> start) {
    if (m_initNode) {
        if (start == m_initNode->getValues()) {
            Logging::debug("Equal start node, tree will be expanded", this);
            return true;
        } else {
            Logging::info("New start node, new tree will be created", this);
            m_graph = m_graph->createEmptyGraphClone();
        }
    }

    if (!m_validityChecker->check(start)) {
        Logging::warning("Init Node could not be connected", this);
        return false;
    }

    m_evaluator->initialize();
    m_initNode = std::make_shared<Node<dim>>(start);
    m_graph->addNode(m_initNode);
    return true;
}

/*!
*  \brief      Return all nodes of the final path
*  \author     Sascha Kaden
*  \param[out] nodes of the path
*  \date       2017-06-20
*/
template <unsigned int dim>
std::vector<std::shared_ptr<Node<dim>>> TreePlanner<dim>::getPathNodes() {
    std::vector<std::shared_ptr<Node<dim>>> nodes;
    if (!m_pathPlanned)
        return nodes;

    nodes.push_back(m_goalNode);
    for (std::shared_ptr<Node<dim>> temp = m_goalNode->getParentNode(); temp != nullptr; temp = temp->getParentNode())
        nodes.push_back(temp);

    std::reverse(nodes.begin(), nodes.end());

    Logging::info("Path has: " + std::to_string(nodes.size()) + " nodes", this);
    return nodes;
}

/*!
*  \brief      Return all points of the final path
*  \author     Sascha Kaden
*  \param[in]  trajectoryStepSize of the result path
*  \param[out] configurations of the path
*  \date       2017-06-20
*/
template <unsigned int dim>
std::vector<Vector<dim>> TreePlanner<dim>::getPath(double posRes, double oriRes) {
    std::vector<Vector<dim>> path;
    if (!m_pathPlanned) {
        Logging::warning("Path is not complete", this);
        return path;
    }

    std::vector<std::shared_ptr<Node<dim>>> nodes = getPathNodes();
    path = this->getPathFromNodes(nodes, posRes, oriRes);

    Logging::info("Path has: " + std::to_string(path.size()) + " points", this);
    return path;
}

/*!
*  \brief      Return the init Node<dim> of the RRTPlanner
*  \author     Sascha Kaden
*  \param[out] init Node
*  \date       2017-06-20
*/
template <unsigned int dim>
std::shared_ptr<Node<dim>> TreePlanner<dim>::getInitNode() const {
    return m_initNode;
}

/*!
*  \brief      Return the goal Node<dim> of the RRTPlanner
*  \author     Sascha Kaden
*  \param[out] goal Node
*  \date       2017-06-20
*/
template <unsigned int dim>
std::shared_ptr<Node<dim>> TreePlanner<dim>::getGoalNode() const {
    return m_goalNode;
}

} /* namespace ippp */

#endif /* TREETPLANNER_HPP */
