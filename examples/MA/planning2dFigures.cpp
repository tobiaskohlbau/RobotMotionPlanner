#include <chrono>

#include <Drawing2D.hpp>
#include <gflags/gflags.h>
#include <ippp/Core.h>
#include <ippp/Environment.h>
#include <ippp/Planner.h>
#include <ippp/UI.h>

using namespace ippp;

DEFINE_string(assetsDir, "../assets", "assets directory");
const unsigned int dim = 2;
const double stepSize = 40;

Mesh generateMap(const std::string& seed) {
    const unsigned int dim = 2;
    Vector2 min(0, 0);
    Vector2 max(500, 500);
    auto sampler = std::make_shared<SamplerRandom<dim>>(std::make_pair(min, max), seed);

    cad::MapGenerator<dim> mapGenerator(std::make_pair(min, max), sampler);
    auto meshes = mapGenerator.generateMap(150, Vector2(10, 10), Vector2(20, 20));
    auto mesh = cad::mergeMeshes(meshes);
    cad::exportCad(cad::ExportFormat::OBJ, "obstacle", mesh);
    return mesh;
}

template <unsigned int dim>
ModuleConfigurator<dim> getCreator() {
    EnvironmentConfigurator envConfigurator;
    envConfigurator.setWorkspaceProperties(AABB(Vector3(0, 0, 0), Vector3(500, 500, 1)));
    // envConfigurator.addObstacle(FLAGS_assetsDir + "/spaces/random2D.obj");
    generateMap("sfdwefno23423");
    envConfigurator.addObstacle("obstacle.obj");
    envConfigurator.setRobotType(RobotType::Point);
    auto environment = envConfigurator.getEnvironment();

    ModuleConfigurator<dim> creator;
    creator.setEnvironment(environment);
    creator.setPathModifierType(PathModifierType::NodeCut);
    creator.setVadilityCheckerType(ValidityCheckerType::Dim2);
    creator.setGraphSortCount(3000);
    // creator.setEvaluatorType(EvaluatorType::TreeConfig);
    creator.setEvaluatorType(EvaluatorType::PRMPose);
    creator.setEvaluatorProperties(stepSize, 30);
    creator.setSamplerType(SamplerType::Uniform);
    creator.setSamplerProperties("slkasjdfsaldfj234;lkj", 1);
    creator.setSamplingProperties(10, 80);
    return creator;
}

int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    Logging::setLogLevel(LogLevel::trace);

    auto PRMcreator = getCreator<dim>();
    auto RRTcreator = getCreator<dim>();
    auto RRTSTARcreator = getCreator<dim>();

    auto prm = std::make_shared<PRM<dim>>(PRMcreator.getEnvironment(), PRMcreator.getPRMOptions(stepSize), PRMcreator.getGraph());
    auto rrt = std::make_shared<RRT<dim>>(RRTcreator.getEnvironment(), RRTcreator.getRRTOptions(stepSize), RRTcreator.getGraph());
    auto rrtStar = std::make_shared<RRTStar<dim>>(RRTSTARcreator.getEnvironment(), RRTSTARcreator.getRRTOptions(stepSize),
                                                  RRTSTARcreator.getGraph());

    Vector2 start(10, 10);
    Vector2 goal(490, 490);

    bool PRMconnected = prm->computePath(start, goal, 1000, 1);
    bool RRTconnected = rrt->computePath(start, goal, 1000, 1);
    bool RRTSTARconnected = rrtStar->computePath(start, goal, 1000, 1);

    auto workspace2D = cad::create2dspace(RRTcreator.getEnvironment()->getSpaceBoundary(), 255);
    cv::Mat image = drawing::eigenToCV(workspace2D.first);
    cv::cvtColor(image, image, CV_GRAY2BGR);
    for (const auto& obstacle : RRTcreator.getEnvironment()->getObstacles())
        drawing::drawPolygons(image, obstacle->model->m_mesh, obstacle->getPose(), workspace2D.second, Vector3i(50, 50, 50));
    cv::namedWindow("PRM", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("RRT", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("RRTSTAR", CV_WINDOW_AUTOSIZE);
    cv::Mat PRMimage = image.clone();
    cv::Mat RRTimage = image.clone();
    cv::Mat RRTSTARimage = image.clone();

    // PRM
    auto PRMnodes = prm->getGraphNodes();
    auto RRTnodes = rrt->getGraphNodes();
    auto RRTSTARnodes = rrtStar->getGraphNodes();
    drawing::drawGraph2D(PRMimage, PRMnodes, Vector3i(125, 125, 200), Vector3i(125, 125, 200), 1);
    drawing::drawGraph2D(RRTimage, RRTnodes, Vector3i(125, 125, 200), Vector3i(125, 125, 200), 1);
    drawing::drawGraph2D(RRTSTARimage, RRTSTARnodes, Vector3i(125, 125, 200), Vector3i(125, 125, 200), 1);

    // std::vector<std::shared_ptr<Node<dim>>> nodes = planner->getGraphNodes();
    // drawing::drawGraph2D(image, nodes, Vector3i(125, 125, 200), Vector3i(125, 125, 200), 1);
    // drawing::drawTree2D(image, nodes, Vector3i(0, 0, 255), Vector3i(125, 125, 200), 1);

    if (PRMconnected)
        drawing::drawPath2D(PRMimage, prm->getPath(), Vector3i(255, 0, 0), 2);
    if (RRTconnected)
        drawing::drawPath2D(RRTimage, rrt->getPath(), Vector3i(255, 0, 0), 2);
    if (RRTSTARconnected)
        drawing::drawPath2D(RRTSTARimage, rrt->getPath(), Vector3i(255, 0, 0), 2);

    cv::imshow("PRM", PRMimage);
    cv::imshow("RRT", RRTimage);
    cv::imshow("RRTSTAR", RRTSTARimage);
    cv::imwrite("PRM.png", PRMimage);
    cv::imwrite("RRT.png", RRTimage);
    cv::imwrite("RRTSTAR.png", RRTSTARimage);

    Stats::writeData(std::cout);
    cv::waitKey(0);
}
