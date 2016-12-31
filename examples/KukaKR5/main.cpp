#include <chrono>
#include <iostream>
#include <memory>

#include <Eigen/Core>

#include <core/utility/Logging.h>
#include <core/utility/Utility.h>
#include <pathPlanner/NormalRRTPlanner.hpp>
#include <pathPlanner/StarRRTPlanner.hpp>
#include <robot/KukaKR5.h>

#include <ui/Writer.hpp>

using namespace rmpl;

int main(int argc, char** argv) {
    Logging::setOutputFile("output.txt");
    Logging::setLogOutput(LogOutput::terminlAndFile);

    std::shared_ptr<KukaKR5> robot(new KukaKR5());
    RRTOptions options(40, 1);
    StarRRTPlanner<6> planner(robot, options);

    Eigen::Matrix<float, 6 ,1> start = utilVec::Vecf(0, 0, 0, 0, 51, 0);
    robot->saveMeshConfig(start);
    Eigen::Matrix<float, 6 ,1> goal = utilVec::Vecf(150, -60, 90, 0, 82.7, 75.5);

    auto startTime = std::chrono::system_clock::now();
    bool connected = planner.computePath(start, goal, 25000, 2);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime);
    std::cout << "Computation time: " << std::chrono::milliseconds(duration).count() / 1000.0 << std::endl;

    std::vector<std::shared_ptr<Node<6>>> nodes = planner.getGraphNodes();
    std::vector<Eigen::Matrix<float, 6, 1>> graphPoints;
    Logging::info("Init Graph has: " + std::to_string(nodes.size()) + "nodes", "Example");
    for (auto node : nodes)
        graphPoints.push_back(robot->directKinematic(node->getValues()));
    writer::writeVecsToFile<6>(graphPoints, "example.ASC", 10);

    if (connected) {
        Logging::info("Init and goal could be connected!", "Example");
        std::vector<Eigen::Matrix<float, 6 ,1>> pathAngles = planner.getPath(1, true);

        std::vector<std::vector<Eigen::Matrix<float, 6 ,1>>> vecs;
        for (auto angle : pathAngles) {
            std::vector<Eigen::Matrix4f> jointTrafos = robot->getJointTrafos(angle);
            std::vector<Eigen::Matrix<float, 6 ,1>> tmp;
            for (auto joint : jointTrafos)
                tmp.push_back(utilGeo::poseMatToVec(joint));
            vecs.push_back(tmp);
        }
        writer::writeTrafosToFile(vecs, "trafos.txt");

        std::vector<Eigen::Matrix<float, 6, 1>> pathPoints;
        for (auto angles : pathAngles)
            pathPoints.push_back(robot->directKinematic(angles));
        writer::appendVecsToFile<6>(pathPoints, "example.ASC", 10);
    } else {
        Logging::warning("Init and goal could NOT be connected!", "Example");
    }

    return 0;
}
