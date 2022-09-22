module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Loader;

import :Cameras;
import :Config;
import :Materials;
import :Scene;
import Camera;

using namespace YAML;

namespace Yart::Yaml
{
    export auto LoadYaml()
    {
        Node node = LoadFile("../../../../Yart.Engine/scene.yaml");

        std::shared_ptr<Config> config = ParseConfigNode(node["config"]);
        std::shared_ptr<Camera> camera = ParseCameraNode(node["camera"]);
        std::shared_ptr<MaterialMap> materialMap = ParseMaterialsNode(node["materials"]);
        std::shared_ptr<SceneConfig> sceneConfig = ParseSceneNode(node["scene"], *materialMap);

        return std::make_tuple(config, camera, sceneConfig, materialMap);
    }
}