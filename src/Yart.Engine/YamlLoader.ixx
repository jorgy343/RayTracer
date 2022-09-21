module;

#include "yaml-cpp/yaml.h"

export module YamlLoader;

import <memory>;
import <tuple>;

import Camera;
import YamlNodes;

using namespace YAML;

namespace Yart::Yaml
{
	export std::tuple<Config, std::unique_ptr<Camera>, std::unique_ptr<SceneConfig>> LoadYaml()
	{
		Node node = LoadFile("../../../../Yart.Engine/scene.yaml");

		Config config = ParseConfigNode(node["config"]);
		std::unique_ptr<Camera> camera = ParseCameraNode(node["camera"]);
		MaterialMap materialMap = ParseMaterialsNode(node["materials"]);
        std::unique_ptr<SceneConfig> sceneConfig = ParseSceneNode(node["scene"], materialMap);

		return std::make_tuple(config, std::move(camera), std::move(sceneConfig));
	}
}