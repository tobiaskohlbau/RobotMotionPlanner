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

#ifndef STATSPROPERTYCOLLECTOR_H
#define STATSPROPERTYCOLLECTOR_H

#include <mutex>

#include <ippp/statistic/StatsCollector.h>
#include <ippp/types.h>

namespace ippp {

class StatsPropertyCollector : public StatsCollector {
  public:
    StatsPropertyCollector(const std::string &name);

    void setProperties(RobotType robotType, unsigned int dim, bool useObstacle, bool useConstraint, bool optimized, double stepSize,
                       SamplerType samplerType, SamplingType samplingType, PlannerType plannerType,
                       PathModifierType pathModifierType, std::pair<Vector6,Vector6> C);

    void initialize();
    nlohmann::json serialize();
    void writeData(std::ostream &stream);

  private:
    bool m_useObstacle;
    bool m_useConstraint;
    bool m_optimized;
    double m_stepSize;
    SamplerType m_samplerType;
    SamplingType m_samplingType;
    PlannerType m_plannerType;
    PathModifierType m_pathModifierType;
    RobotType m_robotType;
    unsigned int m_dim;
    std::pair<Vector6,Vector6> m_C;

    std::mutex m_mutex;
};

} /* namespace ippp */

#endif    // STATSPROPERTYCOLLECTOR_H