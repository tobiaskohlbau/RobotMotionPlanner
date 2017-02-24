#include <chrono>
#include <iostream>
#include <memory>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <Eigen/Core>

#include <core/module/collisionDetection/CollisionDetection2D.hpp>
#include <core/module/collisionDetection/CollisionDetectionTriangleRobot.hpp>
#include <core/utility/Logging.h>
#include <pathPlanner/NormalRRTPlanner.hpp>
#include <pathPlanner/PRMPlanner.hpp>
#include <pathPlanner/RRTStarPlanner.hpp>
#include <robot/model/ModelFactoryTriangle2D.h>
#include <robot/PointRobot.h>
#include <robot/TriangleRobot2D.h>

#include <modelDirectory.h>

#include <ui/Drawing2D.hpp>
#include <ui/Writer.hpp>

using namespace rmpl;

cv::Mat obstacleWorkspace;

void testTriangleRobot(Vector2 minimum, Vector2 maximum, Eigen::MatrixXi mat) {
    const unsigned int dim = 3;
    Vector3 min = utilVec::append<2>(minimum, 0.0);
    Vector3 max = utilVec::append<2>(maximum, 360.0);

    std::vector<Triangle2D> triangles;
    triangles.push_back(Triangle2D(Vector2(0, 0), Vector2(25, 0 ), Vector2(25, 50)));
    triangles.push_back(Triangle2D(Vector2(0, 0), Vector2(0 , 50), Vector2(25, 50)));
    ModelFactoryTriangle2D factory;
    std::shared_ptr<ModelContainer> baseModel = factory.createModel(triangles);
    std::shared_ptr<TriangleRobot2D> triangleRobot(new TriangleRobot2D(baseModel, min, max));
    std::shared_ptr<ModelContainer> model(new Model2D(mat));
    triangleRobot->setWorkspace(model);

    std::shared_ptr<CollisionDetection<3>> collision(new CollisionDetectionTriangleRobot(triangleRobot));
    PRMOptions<dim> prmOptions(30, 0.5, collision, SamplerMethod::randomly);
    RRTOptions<dim> rrtOptions(30, 0.5, collision, SamplerMethod::randomly, SamplingStrategy::normal);

    std::shared_ptr<rmpl::Planner<dim>> planner;
    // planner = std::shared_ptr<PRMPlanner<dim>>(new PRMPlanner<dim>(triangleRobot, prmOptions));
    planner = std::shared_ptr<RRTStarPlanner<dim>>(new RRTStarPlanner<dim>(triangleRobot, rrtOptions));
    // planner = std::shared_ptr<NormalRRTPlanner<dim>>(new NormalRRTPlanner<dim>(triangleRobot, rrtOptions));

    auto startTime = std::chrono::system_clock::now();
    Vector3 start(5, 5, 0);
    Vector3 goal(400.0, 930.0, 50);
    bool connected = planner->computePath(start, goal, 20000, 8);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime);
    std::cout << "Computation time: " << std::chrono::milliseconds(duration).count() / 1000.0 << std::endl;

    std::vector<std::shared_ptr<Node<dim>>> nodes = planner->getGraphNodes();
    if (connected) {
        Logging::info("Init and goal could be connected!", "Example");
        std::vector<Vector3> path = planner->getPath(80, true);

        // Writer::writeVecsToFile(path, "result.txt");

        cv::Mat image = obstacleWorkspace.clone();
        cv::cvtColor(image, image, CV_GRAY2BGR);
        drawing::drawTrianglePath(path, triangles, image, Eigen::Vector3i(0, 0, 255), 2);
        writer::writeVecsToFile<3>(path, "test.txt", 1);

        cv::namedWindow("pathPlanner", CV_WINDOW_AUTOSIZE);
        cv::imshow("pathPlanner", image);
        cv::imwrite("result.png", image);
        cv::waitKey(0);
    }
}

void testPointRobot(Vector2 min, Vector2 max, Eigen::MatrixXi mat) {
    const unsigned int dim = 2;
    std::shared_ptr<PointRobot> robot(new PointRobot(min, max));
    std::shared_ptr<ModelContainer> model(new Model2D(mat));
    robot->setWorkspace(model);

    std::shared_ptr<CollisionDetection<2>> collision(new CollisionDetection2D(robot));
    PRMOptions<dim> prmOptions(40, 0.5, collision, SamplerMethod::randomly, SamplingStrategy::nearObstacles);
    RRTOptions<dim> rrtOptions(50, 1, collision, SamplerMethod::randomly, SamplingStrategy::nearObstacles);

    std::shared_ptr<rmpl::Planner<dim>> planner;
    // planner = std::shared_ptr<PRMPlanner<dim>>(new PRMPlanner<dim>(robot, prmOptions));
    planner = std::shared_ptr<RRTStarPlanner<dim>>(new RRTStarPlanner<dim>(robot, rrtOptions));
    // planner = std::shared_ptr<NormalRRTPlanner<dim>>(new NormalRRTPlanner<dim>(robot, rrtOptions));

    // compute the tree
    auto startTime = std::chrono::system_clock::now();
    Vector2 start(50.0, 30.0);
    Vector2 goal(870.0, 870.0);
    bool connected = planner->computePath(start, goal, 5200, 2);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime);
    std::cout << "Computation time: " << std::chrono::milliseconds(duration).count() / 1000.0 << std::endl;
    std::vector<std::shared_ptr<Node<dim>>> nodes = planner->getGraphNodes();

    cv::Mat image = obstacleWorkspace.clone();
    cv::cvtColor(image, image, CV_GRAY2BGR);

    drawing::drawGraph2D(nodes, image, Eigen::Vector3i(125, 125, 125), Eigen::Vector3i(125, 125, 125), 1);
    drawing::drawTree2D(nodes, image, Eigen::Vector3i(0, 0, 255), Eigen::Vector3i(0, 0, 0), 1);

    if (connected) {
        std::vector<Vector2> pathPoints = planner->getPath(1, true);
        drawing::drawPath2D(pathPoints, image, Eigen::Vector3i(255, 0, 0), 3);
        writer::writeVecsToFile<2>(pathPoints, "test.txt", 1);
    }

    cv::namedWindow("pathPlanner", CV_WINDOW_AUTOSIZE);
    cv::imshow("pathPlanner", image);
    cv::imwrite("result.png", image);
    cv::waitKey(0);
}

int main(int argc, char** argv) {
    // obstacleWorkspace = cv::imread(getModelDirectory() + "spaces/freeWorkspace.png", CV_LOAD_IMAGE_GRAYSCALE);
    // obstacleWorkspace = cv::imread(getModelDirectory() + "spaces/labyrinth.png", CV_LOAD_IMAGE_GRAYSCALE);
    obstacleWorkspace = cv::imread(getModelDirectory() + "spaces/obstacleWorkspace.png", CV_LOAD_IMAGE_GRAYSCALE);
    Eigen::MatrixXi mat = drawing::cvToEigen(obstacleWorkspace);

    Vector2 minBoundary(0.0, 0.0);
    Vector2 maxBoundary(mat.rows(), mat.cols());

    testTriangleRobot(minBoundary, maxBoundary, mat);

    testPointRobot(minBoundary, maxBoundary, mat);
}
