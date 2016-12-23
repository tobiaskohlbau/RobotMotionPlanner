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

#ifndef KUKAKR5_H_
#define KUKAKR5_H_

#include <robot/SerialRobot.h>

namespace rmpl {

/*!
* \brief   Class for the Kuka KR5 robot
* \author  Sascha Kaden
* \date    2016-10-22
*/
class KukaKR5 : public SerialRobot {
  public:
    KukaKR5();
    Eigen::Matrix<float, 6, 1> directKinematic(const Eigen::VectorXf &angles);
    std::vector<Eigen::Matrix4f> getJointTrafos(const Eigen::VectorXf &angles);
};

} /* namespace rmpl */

#endif /* KUKAKR5_H_ */
