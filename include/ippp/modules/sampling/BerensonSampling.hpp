//-------------------------------------------------------------------------//
//
// Copyright 2018 Sascha Kaden
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

#ifndef BERENSONSAMPLING_HPP
#define BERENSONSAMPLING_HPP

#include <ippp/environment/robot/SerialRobot.h>
#include <ippp/modules/constraint/Constraint.hpp>
#include <ippp/modules/distanceMetrics/DistanceMetric.hpp>
#include <ippp/modules/sampling/Sampling.hpp>

namespace ippp {

/*!
* \brief   Class SamplingNearObstacle creates samples with the passed Sampler and if they in collision valid samples in
* neighborhood will found.
* \author  Sascha Kaden
* \date    2016-12-20
*/
template <unsigned int dim>
class BerensonSampling : public Sampling<dim> {
  public:
    BerensonSampling(const std::shared_ptr<Environment> &environment, const std::shared_ptr<Graph<dim>> &graph,
                     const std::shared_ptr<Sampler<dim>> &sampler, const std::shared_ptr<Constraint<dim>> &constraint,
                     const std::shared_ptr<ValidityChecker<dim>> &validityChecker,
                     const std::shared_ptr<DistanceMetric<dim>> &metric, size_t attempts, double stepSize,
                     const std::string &name = "BerensonSampling");

    Vector<dim> getSample() override;
    Vector<dim> getSample(const Vector<dim> &prevSample) override;

  private:
    bool project(Vector<dim> &config, const Vector<dim> &nearNode);

    std::shared_ptr<SerialRobot> m_serialRobot = nullptr;
    std::shared_ptr<Constraint<dim>> m_constraint = nullptr;
    std::shared_ptr<DistanceMetric<dim>> m_metric = nullptr;
    std::pair<Vector<dim>, Vector<dim>> m_C;
    double m_stepSize = 1;
    double m_simplifiedStepSize = 1;

    using Sampling<dim>::m_attempts;
    using Sampling<dim>::m_graph;
    using Sampling<dim>::m_sampler;
    using Sampling<dim>::m_validityChecker;
};

/*!
*  \brief      Constructor of the class Sampling
*  \author     Sascha Kaden
*  \param[in]  Environment
*  \param[in]  CollisionDetection
*  \param[in]  TrajectoryPlanner
*  \param[in]  Sampler
*  \param[in]  attempts for one sampling
*  \date       2016-12-20
*/
template <unsigned int dim>
BerensonSampling<dim>::BerensonSampling(const std::shared_ptr<Environment> &environment, const std::shared_ptr<Graph<dim>> &graph,
                                        const std::shared_ptr<Sampler<dim>> &sampler,
                                        const std::shared_ptr<Constraint<dim>> &constraint,
                                        const std::shared_ptr<ValidityChecker<dim>> &validityChecker,
                                        const std::shared_ptr<DistanceMetric<dim>> &metric, size_t attempts, double stepSize,
                                        const std::string &name)
    : Sampling<dim>(name, environment, graph, sampler, validityChecker, attempts),
      m_constraint(constraint),
      m_metric(metric),
      m_serialRobot(std::dynamic_pointer_cast<SerialRobot>(environment->getRobot())),
      m_stepSize(stepSize),
      m_simplifiedStepSize(stepSize) {
    m_metric->simplifyDist(m_simplifiedStepSize);
}

/*!
*  \brief      Sample in the neighborhood of obstacles
*  \details    If Sample is in collision, second random collision valid sample will be computed and by binary search the
*              nearest collision valid sample to the first sample, will be taken.
*  \author     Sascha Kaden
*  \param[out] sample Vector
*  \date       2016-12-20
*/
template <unsigned int dim>
Vector<dim> BerensonSampling<dim>::getSample() {
    Vector<dim> qRand = m_sampler->getSample();
    if (m_validityChecker->check(qRand))
        return qRand;

    auto nodeNear = m_graph->getNearestNode(qRand);
    auto nodeNearValues = nodeNear->getValues();
    double dist = m_metric->calcDist(qRand, nodeNearValues);
    qRand = nodeNearValues + (std::min(m_stepSize, dist) * (qRand - nodeNearValues) / dist);
    if (project(qRand, nodeNearValues))
        if (m_validityChecker->check(qRand))
            return qRand;

    return util::NaNVector<dim>();
}

template <unsigned int dim>
Vector<dim> BerensonSampling<dim>::getSample(const Vector<dim> &prevSample) {
    return getSample();
}

template <unsigned int dim>
bool BerensonSampling<dim>::project(Vector<dim> &config, const Vector<dim> &nearNode) {
    for (size_t i = 0; i < m_attempts; ++i) {
        if (m_constraint->check(config))
            return true;

        Vector6 eucErr = m_constraint->calcEuclideanError(config);
        auto J = m_serialRobot->calcJacobian(config);
        MatrixX invJ = J.completeOrthogonalDecomposition().pseudoInverse();
        config -= invJ * eucErr;    // qs = qs - qerr
        if (!m_constraint->checkRobotBound(config) || m_metric->calcSimpleDist(config, nearNode) > 2 * m_simplifiedStepSize)
            return false;
    }
    return false;
}

} /* namespace ippp */

#endif /* BERENSONSAMPLING_HPP */