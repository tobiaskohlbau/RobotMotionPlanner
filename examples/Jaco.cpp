#include <ctime>

#include <gflags/gflags.h>

#include <ippp/Core.h>
#include <ippp/Environment.h>
#include <ippp/Planner.h>
#include <ippp/UI.h>

using namespace ippp;

DEFINE_string(assetsDir, "../assets", "assets directory");

void simpleRRT() {
    const unsigned int dim = 6;
    EnvironmentConfigurator envConfigurator;

    envConfigurator.setWorkspaceProperties(AABB(Vector3(-1000, -1000, -5), Vector3(1000, 1000, 1500)));
    std::string objDir = FLAGS_assetsDir + "/spaces/3D/fairObjects/";
    envConfigurator.addObstacle(objDir + "base.obj");
    envConfigurator.addObstacle(objDir + "table.obj");
    envConfigurator.addObstacle(objDir + "shelve.obj", util::Vecd(-850,0,0,0,0,0));

    Vector6 minRobotBound = util::Vecd(-370, -370, -370, -370, -370, -370);
    minRobotBound = util::toRad<6>(minRobotBound);
    std::vector<DhParameter> dhParameters({DhParameter(util::halfPi(), 0, 275.5), DhParameter(util::pi(), 410, 0),
                                           DhParameter(util::halfPi(), 0, -9.8), DhParameter(0.95993, 0, -249.18224),
                                           DhParameter(0.95993, 0, -83.76448), DhParameter(util::pi(), 0, -210.58224)});
    std::string jaco = FLAGS_assetsDir + "/robotModels/Jaco/";
    std::vector<std::string> linkModelFiles = {jaco + "jaco_1.obj", jaco + "jaco_2.obj", jaco + "jaco_3.obj",
                                               jaco + "jaco_4.obj", jaco + "jaco_5.obj", jaco + "jaco_gripper.obj"};
    envConfigurator.setRobotBaseModelFile(jaco + "jaco_0.obj");
    envConfigurator.setRobotType(RobotType::Jaco);

    envConfigurator.setFactoryType(FactoryType::ModelFCL);
    std::vector<DofType> dofTypes(6, DofType::jointRot);
    envConfigurator.setRobotBaseProperties(dim, dofTypes, std::make_pair(minRobotBound, -minRobotBound));
    std::vector<Vector6> linkOffsets(6);
    linkOffsets[0] = util::Vecd(0, 0, 160, 0, 0, 0);
    linkOffsets[3] = util::Vecd(-3, 4, -207.3, 0, 0, 0);
    linkOffsets[4] = util::Vecd(0, 0, -37.05, 0, 0, 0);
    linkOffsets[5] = util::Vecd(0, 12, -37.05, 0, 0, 0);
    auto linkTransforms = util::toTransform(linkOffsets);
    envConfigurator.setSerialRobotProperties(dhParameters, linkModelFiles, linkTransforms);

    auto environment = envConfigurator.getEnvironment();
    envConfigurator.saveConfig("jacoEnv.json");
    auto serialRobot = std::dynamic_pointer_cast<Jaco>(environment->getRobot());

    // std::vector<Transform> transforms(6, Transform::Identity());
    // ui::save("transforms.json", jsonSerializer::serialize(transforms));
    // auto models = serialRobot->getLinkModels();
    // std::vector<Transform> transforms = jsonSerializer::deserializeTransforms(ui::loadJson("transforms.json"));
    // for (size_t i = 0; i < dim; ++i) {
    //    std::cout << transforms[i].matrix() << std::endl;
    //    models[i]->transformModel(transforms[i]);
    //    cad::exportCad(cad::ExportFormat::OBJ, "jaco_" + std::to_string(i+1), models[i]->m_mesh);
    //}

    // Vector<dim> testConfig = util::Vecd(-90, 90, 170, 30, 90, 90);
    // Vector<dim> testConfig = util::Vecd(45, 90, 170, 30, 10, 120);
    // Vector<dim> testConfig = util::Vecd(180, 180, 180, 180, 180, 180);
    // testConfig = util::toRad<dim>(testConfig);
    // Vector<dim> testConfig = util::Vecd(0, util::halfPi(), -util::halfPi(), 0, util::pi(), -util::halfPi());
    // serialRobot->saveMeshConfig(testConfig);

    double stepSize = 2;
    ModuleConfigurator<dim> creator;
    creator.setEvaluatorType(EvaluatorType::TreeConnect);
    creator.setEvaluatorProperties(stepSize, 500);
    creator.setGraphSortCount(3000);
    creator.setEnvironment(environment);
    creator.setValidityCheckerType(ValidityCheckerType::FclSerial);
    creator.setSamplingType(SamplingType::NearObstacle);

    RRTStarConnect<dim> planner(environment, creator.getRRTOptions(stepSize), creator.getGraph(), creator.getGraphB());
    //Vector<dim> start = util::Vecd(180, 180, 180, 180, 180, 180);
    //Vector<dim> goal = util::Vecd(-90, 90, 150, 30, 35, 114);
    //start = util::toRad<dim>(start);
    //goal = util::toRad<dim>(goal);
    //bool connected = planner.computePath(start, goal, 6000, 24);


    auto configs = jsonSerializer::deserializeVectors<dim>(ui::loadJson("configs.json"));
    for (auto &config : configs)
        config=util::toRad<dim>(config);
    std::vector<bool> results(configs.size()-1, false);
    std::vector<Vector6> path;
    for (size_t i = 0; i < configs.size()-1; ++i) {
        results[i] = planner.computePath(configs[i], configs[i+1], 6000, 24);
        //serialRobot->saveMeshConfig(configs[i+1]);
        if (results[i]) {
            std::cout << "Init and goal could be connected!" << std::endl;
            auto tmpPath = planner.getPath(0.001, 0.001);
            auto json = jsonSerializer::serialize<dim>(tmpPath);
            ui::save(std::to_string(i) + ".json", json);
            path.insert(path.end(), tmpPath.begin(), tmpPath.end());
        }
    }
    auto json = jsonSerializer::serialize<dim>(path);
    ui::save("jacoPath.json", json);
}

int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    Logging::setLogLevel(LogLevel::trace);

    simpleRRT();
//    Stats::writeData(std::cout);
//    std::string string;
//    std::cin >> string;

    return 0;
}
