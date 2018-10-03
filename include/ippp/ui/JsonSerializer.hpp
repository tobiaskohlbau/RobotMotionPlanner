//-------------------------------------------------------------------------//
//
// Copyright 2018 Sascha Kaden
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

#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include <string>
#include <vector>

#include <json.hpp>

#include <ippp/dataObj/DhParameter.h>
#include <ippp/environment/robot/RobotBase.h>
#include <ippp/types.h>
#include <ippp/util/Logging.h>

namespace ippp {
namespace jsonSerializer {

std::string serialize(const nlohmann::json &data);

nlohmann::json serialize(const Transform &transform);
Transform deserializeTransform(const nlohmann::json &data);

nlohmann::json serialize(const std::vector<Transform> &configs);
std::vector<Transform> deserializeTransforms(const nlohmann::json &data);

nlohmann::json serialize(const std::vector<DhParameter> &parameters);
std::vector<DhParameter> deserializeDhParameters(const nlohmann::json &data);

nlohmann::json serialize(const VectorX &vector);
VectorX deserializeVector(const nlohmann::json &data);

nlohmann::json serialize(const AABB &aabb);
AABB deserializeAABB(const nlohmann::json &data);

nlohmann::json serialize(const std::vector<DofType> &dofTypes);
std::vector<DofType> deserializeDofTypes(const nlohmann::json &data);

nlohmann::json serialize(const std::pair<Vector6, Vector6> &C);
std::pair<Vector6, Vector6> deserializeC(const nlohmann::json &data);

/*!
    *  \brief      Serialize vectors to a nlohmann::json
    *  \param[in]  vector of configurations
    *  \param[in]  scale
    *  \param[out] nlohmann::json
    *  \author     Sascha Kaden
    *  \date       2017-11-30
    */
template <unsigned int dim>
nlohmann::json serialize(const std::vector<Vector<dim>> &configs, double scale = 1) {
    if (configs.empty())
        return std::string();

    nlohmann::json json;
    json["Dimension"] = dim;
    json["NumberConfigurations"] = configs.size();

    std::vector<std::vector<double>> data;
    for (const auto &config : configs) {
        Vector<dim> tempConfig = scale * config;
        data.push_back(std::vector<double>(tempConfig.data(), tempConfig.data() + tempConfig.size()));
    }
    json["data"] = data;
    return json;
}

/*!
*  \brief      Deserialize nlohmann::json to a std::vector of Vector<dim>
*  \param[in]  nlohmann::json
*  \param[out] vector of configurations
*  \author     Sascha Kaden
*  \date       2017-11-30
*/
template <unsigned int dim>
std::vector<Vector<dim>> deserializeVectors(const nlohmann::json &data) {
    std::vector<Vector<dim>> vectors;
    if (data.empty())
        return vectors;

    if (data["Dimension"].get<unsigned int>() != dim) {
        Logging::error("Data has wrong dimension", "JsonSerializer");
        return vectors;
    }

    std::vector<std::vector<double>> stdVectors = data["data"].get<std::vector<std::vector<double>>>();
    size_t size = data["NumberConfigurations"].get<size_t>();
    if (stdVectors.size() != size) {
        Logging::error("Wrong vector size of file", "JsonSerializer");
        return vectors;
    }

    for (const auto &vec : stdVectors) {
        Vector<dim> eigenVec;
        for (unsigned int j = 0; j < dim; ++j)
            eigenVec[j] = vec[j];
        vectors.push_back(eigenVec);
    }
    return vectors;
}

} /* namespace jsonSerializer */
} /* namespace ippp */

#endif    // JSONSERIALIZER_H