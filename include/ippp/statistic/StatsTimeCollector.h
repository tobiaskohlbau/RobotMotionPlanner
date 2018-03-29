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

#ifndef STATSTIMECOLLECTOR_H
#define STATSTIMECOLLECTOR_H

#include <mutex>

#include <ippp/statistic/StatsCollector.h>
#include <ippp/statistic/StatsTimeContainer.h>

namespace ippp {

/*!
* \brief   Statistics class to collect stats and write them
* \author  Sascha Kaden
* \date    2017-10-20
*/
class StatsTimeCollector : public StatsCollector {
  public:
    StatsTimeCollector(const std::string &name);
    void start();
    void stop();

    virtual void initialize();
    virtual nlohmann::json serialize();
    void writeData(std::ostream &stream);

  private:
    std::chrono::system_clock::time_point m_start;
    std::chrono::system_clock::time_point m_stop;
    std::mutex m_mutex;
};

} /* namespace ippp */

#endif    // STATSCOLLISIONCOLLECTOR_H
