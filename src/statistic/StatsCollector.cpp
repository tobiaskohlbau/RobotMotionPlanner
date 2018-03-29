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

#include <ippp/statistic/StatsCollector.h>

namespace ippp {

StatsCollector::StatsCollector(const std::string &name) : Identifier(name) {
}

void StatsCollector::addContainer(const std::shared_ptr<StatsContainer> &container) {
    if (!container)
        return;

    m_containers.push_back(container);
}

std::shared_ptr<StatsContainer> StatsCollector::getContainer(size_t hash) {
    for (auto &container : m_containers)
        if (hash == container->getHash())
            return container;

    return nullptr;
}

} /* namespace ippp */
