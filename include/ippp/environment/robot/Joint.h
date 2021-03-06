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

#ifndef JOINT_H
#define JOINT_H

#include <memory>

#include <ippp/environment/model/ModelContainer.h>

namespace ippp {

/*!
* \brief   Joint class contains the boundaries of a joint and the MeshContainer from it
* \author  Sascha Kaden
* \date    2016-08-25
*/
class Joint {
  public:
    Joint();
    Joint(double minBound, double maxBound);
    Joint(double minBound, double maxBound, std::shared_ptr<ModelContainer> model);

    void setModel(std::shared_ptr<ModelContainer> &model);
    std::shared_ptr<ModelContainer> getModel() const;

    void setBoundaries(double minBound, double maxBound);
    std::pair<double, double> getBoundaries() const;

  private:
    double m_minBound = 0;
    double m_maxBound = 0;
    std::shared_ptr<ModelContainer> m_model = nullptr;
};

} /* namespace ippp */

#endif    // JOINT_H
