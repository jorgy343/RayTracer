module;

#include "yaml-cpp/yaml.h"

export module YamlLoader;

import <memory>;

import Camera;
import YamlNodes;

using namespace YAML;

namespace Yart::Yaml
{
	export Config LoadYaml()
	{
		Node node = LoadFile("../../../../Yart.Engine/scene.yaml");

		Config config = ParseConfigNode(node["config"]);
		std::unique_ptr<Camera> camera = ParseCameraNode(node["camera"]);
		MaterialMap materialMap = ParseMaterialsNode(node["materials"]);

		return config;
	}
}