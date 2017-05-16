#include <chrono>
#include <iostream>
#include <memory>

#include <Eigen/Core>

#include <core/collisionDetection/CollisionDetectionPqp.hpp>
#include <core/utility/Logging.h>
#include <core/utility/Utility.h>
#include <environment/KukaKR5.h>
#include <pathPlanner/RRTStar.hpp>

#include <ui/Writer.hpp>

using namespace ippp;

int main(int argc, char** argv) {
    const unsigned int dim = 6;
    Logging::setOutputFile("output.txt");
    Logging::setLogOutput(LogOutput::terminlAndFile);

    std::shared_ptr<KukaKR5> robot(new KukaKR5());
    std::shared_ptr<CollisionDetection<dim>> collision(new CollisionDetectionPqp<dim>(robot));
    std::shared_ptr<TrajectoryPlanner<dim>> trajectory(new TrajectoryPlanner<dim>(collision));
    std::shared_ptr<Sampler<dim>> sampler(new Sampler<dim>(robot));
    std::shared_ptr<Sampling<dim>> sampling(new Sampling<dim>(robot, collision, trajectory, sampler));
    std::shared_ptr<DistanceMetric<dim>> distanceMetric(new DistanceMetric<dim>());

    RRTOptions<dim> options(40, collision, trajectory, sampling, distanceMetric);
    std::shared_ptr<NeighborFinder<dim, std::shared_ptr<Node<dim>>>> neighborFinder(
        new KDTree<dim, std::shared_ptr<Node<dim>>>(distanceMetric));
    std::shared_ptr<Graph<dim>> graph(new Graph<dim>(4000, neighborFinder));
    RRTStar<dim> planner(robot, options, graph);

    Vector6 start = util::Vecf(0, 0, 0, 0, 51, 0);
    robot->saveMeshConfig(start);
    Vector6 goal = util::Vecf(150, -60, 90, 0, 82.7, 75.5);

    auto startTime = std::chrono::system_clock::now();
    bool connected = planner.computePath(start, goal, 25000, 2);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime);
    std::cout << "Computation time: " << std::chrono::milliseconds(duration).count() / 1000.0 << std::endl;

    std::vector<std::shared_ptr<Node<dim>>> nodes = planner.getGraphNodes();
    std::vector<Vector6> graphPoints;
    Logging::info("Init Graph has: " + std::to_string(nodes.size()) + "nodes", "Example");
    for (auto node : nodes)
        graphPoints.push_back(robot->directKinematic(node->getValues()));
    writer::writeVecsToFile<dim>(graphPoints, "example.ASC", 10);

    if (connected) {
        Logging::info("Init and goal could be connected!", "Example");
        std::vector<Vector6> pathAngles = planner.getPath(1, true);

        std::vector<std::vector<Vector6>> vecs;
        for (auto angle : pathAngles) {
            std::vector<Matrix4> jointTrafos = robot->getJointTrafos(angle);
            std::vector<Vector6> tmp;
            for (auto joint : jointTrafos)
                tmp.push_back(util::poseMatToVec(joint));
            vecs.push_back(tmp);
        }
        writer::writeTrafosToFile(vecs, "trafos.txt");

        std::vector<Vector6> pathPoints;
        for (auto angles : pathAngles)
            pathPoints.push_back(robot->directKinematic(angles));
        writer::appendVecsToFile<dim>(pathPoints, "example.ASC", 10);
    } else {
        Logging::warning("Init and goal could NOT be connected!", "Example");
    }

    return 0;
}
