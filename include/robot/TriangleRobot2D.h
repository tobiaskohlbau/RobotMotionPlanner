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

#ifndef TRIANGLEROBOT2D_H_
#define TRIANGLEROBOT2D_H_

#include <core/dataObj/PointList.hpp>
#include <robot/RobotBase.hpp>

namespace rmpl {

/*!
* \brief   Class for the 2D trianlge robot.
* \details The robot contains the list of triangles, which defines his body.
* \author  Sascha Kaden
* \date    2016-11-14
*/
class TriangleRobot2D : public RobotBase<3> {
  public:
    TriangleRobot2D(std::vector<Triangle2D> triangles, Vector3 minBoundary, Vector3 maxBoundary);

    bool setTriangles(std::vector<Triangle2D> &triangles);
    bool addTriangle(Triangle2D &triangle);
    std::vector<Triangle2D> getTriangles();

  private:
    std::vector<Triangle2D> m_triangles;
};

} /* namespace rmpl */

#endif /* TRIANGLEROBOT2D_H_ */
