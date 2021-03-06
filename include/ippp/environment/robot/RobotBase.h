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

#ifndef ROBOTBASE_H
#define ROBOTBASE_H

#include <string>
#include <vector>

#include <ippp/Identifier.h>
#include <ippp/types.h>
#include <ippp/util/Logging.h>
#include <ippp/util/Utility.h>
#include <ippp/environment/model/ModelContainer.h>

namespace ippp {

enum RobotCategory { serial, mobile };
enum DofType { planarPos, planarRot, volumetricPos, volumetricRot, joint, position, rotation};

/*!
* \brief   Base class of all robots
* \author  Sascha Kaden
* \date    2016-06-30
*/
class RobotBase : public Identifier {
  public:
    virtual ~RobotBase();

  protected:
    RobotBase(const std::string &name, const unsigned int dim, const RobotCategory robotType, const std::pair<VectorX, VectorX> &boundary,
              std::vector<DofType> dofTypes);

  public:
    void setPose(const Vector6 &pose);
    void setPose(const Transform &pose);
    Transform getPose() const;
    virtual Transform getTransformation(const VectorX &config) const = 0;

    void setBaseModel(const std::shared_ptr<ModelContainer> &model);
    std::shared_ptr<ModelContainer> getBaseModel() const;

    VectorX getMinBoundary() const;
    VectorX getMaxBoundary() const;
    unsigned int getDim() const;
    RobotCategory getRobotCategory() const;
    std::vector<DofType> getDofTypes() const;

  protected:
    const RobotCategory m_robotType;
    const VectorX m_minBoundary;
    const VectorX m_maxBoundary;

    const unsigned int m_dim;
    const std::vector<DofType> m_dofTypes;
    Transform m_pose;

    std::shared_ptr<ModelContainer> m_baseModel;
};

} /* namespace ippp */

#endif /* ROBOTBASE_H */
