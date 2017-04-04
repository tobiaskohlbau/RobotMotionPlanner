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

#ifndef MODELCONTAINER_H
#define MODELCONTAINER_H

#include <memory>
#include <vector>

#include <Eigen/Core>

#include <core/types.h>
#include <core/module/Identifier.h>
#include <core/utility/UtilVec.hpp>
#include <robot/model/BoundingBox.h>

namespace rmpl {

/*!
* \brief   Base class for all robot models
* \author  Sascha Kaden
* \date    2017-02-19
*/
class ModelContainer : public Identifier {
  public:
    virtual ~ModelContainer();

  protected:
    ModelContainer(const std::string &name, const BoundingBox &boundingBox = BoundingBox());

  public:
    virtual bool empty() const = 0;

    std::vector<Vector3> m_vertices;
    std::vector<Vector3i> m_faces;
    std::vector<Vector3> m_normals;
    BoundingBox m_boundingBox;
};

} /* namespace rmpl */

#endif    // MODELCONTAINER_H
