#ifndef TRAJECTORYPLANNER_H_
#define TRAJECTORYPLANNER_H_

#include <cstdint>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#include "CollisionDetection.h"
#include "Node.h"

enum class TrajectoryMethod
{
    linear,
    spline,
};

class TrajectoryPlanner
{
public:
    TrajectoryPlanner(const TrajectoryMethod method, CollisionDetection *collision);
    bool computeTrajectory(const std::shared_ptr<Node> node1, const std::shared_ptr<Node> node2);

private:
    TrajectoryMethod m_method;
    CollisionDetection *m_collision;
};

TrajectoryPlanner::TrajectoryPlanner(const TrajectoryMethod method, CollisionDetection *collision) {
    m_method = method;
    m_collision = collision;
}

bool TrajectoryPlanner::computeTrajectory(const std::shared_ptr<Node> node1, const std::shared_ptr<Node> node2) {
    if (node1->getDim() != node2->getDim()) {
        std::cout << "TrajectoryPlanner: Nodes have different dimensions" << std::endl;
        return false;
    }

    if (node1->getDim() == 2) {
        cv::Point pt1(node1->getX(), node1->getY());
        cv::Point pt2(node2->getX(), node2->getY());
        cv::LineIterator it(m_collision->get2DWorkspace(), pt1, pt2, 8);

        for (int i = 0; i < it.count; i++, ++it) {
            uint8_t val = m_collision->get2DWorkspace().at<uint8_t>(it.pos());
            if (val == 0)
                return false;
        }
    }
    return true;
}

#endif /* TRAJECTORYPLANNER_H_ */