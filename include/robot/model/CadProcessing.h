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

#ifndef CADPROCESSING_H
#define CADPROCESSING_H

#include <string>

#include <Eigen/Core>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <core/types.h>

namespace rmpl {

enum class ExportFormat {COLLADA, X_FILES, STEP, OBJ, STEREOLITHOGRAPHY, STEREOLITHOGRAPHY_BINARY, STANFORD_POLYGON_LIBRARY,
    STANFORD_POLYGON_LIBRARY_BINARY, AUTODESK_3DS, GL_TRANSMISSION_FORMAT, GL_TRANSMISSION_FORMAT_BINARY, ASSIMP_BINARY,
    ASSXML_DOCUMENT, EXTENSIBLE_3D};

bool importCad(const std::string &filepath, std::vector<Vector3> &vertices, std::vector<Vector3i> &faces);
bool exportCad(ExportFormat format, const std::string &filePath, const std::vector<Vector3> &vertices, const std::vector<Vector3i> &faces);

aiScene generateScene(const std::vector<Vector3> &vertices, const std::vector<Vector3i> &faces);


} /* namespace rmpl */


#endif //CADPROCESSING_H