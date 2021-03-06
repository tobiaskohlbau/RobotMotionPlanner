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

#ifndef TRIANGLEROBOT2D_H
#define TRIANGLEROBOT2D_H

#include <ippp/environment/model/ModelTriangle2D.h>
#include <ippp/environment/robot/RobotBase.h>

namespace ippp {

/*!
* \brief   Class for the 2D trianlge robot.
* \details The robot contains the list of triangles, which defines his body.
* \author  Sascha Kaden
* \date    2016-11-14
*/
class TriangleRobot2D : public RobotBase {
  public:
    TriangleRobot2D(const std::shared_ptr<ModelContainer> &triangleModel, const std::pair<Vector3, Vector3> &boundary);
    Transform getTransformation(const VectorX &config) const override ;
};

} /* namespace ippp */

#endif /* TRIANGLEROBOT2D_H */
