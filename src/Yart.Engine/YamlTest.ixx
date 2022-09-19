module;

#include "yaml-cpp/yaml.h"

export module YamlTest;

import YamlNodes;

using namespace YAML;

namespace Yart::Yaml
{
	export Config Test()
	{
		Node node = LoadFile("C:/SAVE/repos/Yart/src/Yart.Engine.Lib/scene.yaml");

		Config config = node["config"].as<Config>();

		return config;
	}
}