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

#ifndef MODELPQP_H
#define MODELPQP_H

#include <robot/model/ModelContainer.h>

#include <PQP.h>

namespace rmpl {

/*!
* \brief   Pqp model class
* \author  Sascha Kaden
* \date    2017-02-19
*/
class ModelPqp : public ModelContainer {
  public:
    ModelPqp();
    bool empty() const;
    void transform(const Vector6 &config);
    void transform(const Matrix4 &T);

    PQP_Model m_pqpModel;

  private:
    void updatePqpModel();
};

} /* namespace rmpl */

#endif    // MODELPQP_H
