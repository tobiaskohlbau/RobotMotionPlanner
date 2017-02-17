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

#include <robot/model/ModelPqp.h>

namespace rmpl {

ModelPqp::ModelPqp() {
    m_pqpModel = std::shared_ptr<PQP_Model>(new PQP_Model());
}

bool ModelPqp::empty() const {
    if (m_pqpModel != nullptr)
        return true;
    else
        return false;
}

} /* namespace rmpl */
