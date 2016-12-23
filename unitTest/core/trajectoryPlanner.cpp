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

#include <boost/test/unit_test.hpp>

#include <core/module/TrajectoryPlanner.h>
#include <core/utility/Utility.h>

using namespace rmpl;

BOOST_AUTO_TEST_SUITE(constructor)

BOOST_AUTO_TEST_CASE(computeTrajectory) {
    // init TrajectoryPlanner
    std::shared_ptr<RobotBase> robot = std::shared_ptr<RobotBase>(new RobotBase("", CollisionType::pqp, RobotType::mobile, 6));
    std::shared_ptr<CollisionDetection> collision = std::shared_ptr<CollisionDetection>(new CollisionDetection(robot));
    TrajectoryPlanner planner(0.1, collision);

    // test trajectories
    Eigen::VectorXf init = Vecf(0, 0, 0, 0, 0, 0);
    Eigen::VectorXf goal = Vecf(0, 0, 0, 0, 0, 0);
    std::vector<Eigen::VectorXf> path;
    path = planner.calcTrajectoryCont(init, goal);
    BOOST_CHECK(path.size() == 0);

    goal = Vecf(1, 1, 1, 1, 1, 1);
    path = planner.calcTrajectoryCont(init, goal);
    float dist = 1 / goal.norm() * 0.1;
    for (float i = 0; i < path.size(); ++i) {
        for (int j = 0; j < 6; ++j) {
            BOOST_CHECK(path[i][j] <= dist + 0.0001);
            BOOST_CHECK(path[i][j] >= dist - 0.0001);
        }
        dist += 1 / goal.norm() * 0.1;
    }

    goal = Vecf(-1, -1, -1, -1, -1, -1);
    path = planner.calcTrajectoryCont(init, goal);
    dist = -1 / goal.norm() * 0.1;
    for (float i = 0; i < path.size(); ++i) {
        for (int j = 0; j < 6; ++j) {
            BOOST_CHECK(path[i][j] <= dist + 0.0001);
            BOOST_CHECK(path[i][j] >= dist - 0.0001);
        }
        dist -= 1 / goal.norm() * 0.1;
    }
}

BOOST_AUTO_TEST_SUITE_END()
