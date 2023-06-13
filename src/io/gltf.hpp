#pragma once

#include <string>
#include "drw/SceneData.hpp"

namespace io::gltf {
SceneData* load_scene(std::string path);
}

