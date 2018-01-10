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

#ifndef OBSTACLEOBJECT_H
#define OBSTACLEOBJECT_H

#include <ippp/environment/EnvObject.h>
#include <ippp/environment/model/ModelContainer.h>

namespace ippp {

class ObstacleObject : public EnvObject {
  public:
    ObstacleObject(const std::string &name, std::shared_ptr<ModelContainer> &model);

    std::shared_ptr<ModelContainer> model = nullptr;
};

} /* namespace ippp */

#endif /* OBSTACLEOBJECT_H */