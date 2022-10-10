module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Config;

import "Common.h";

import :Vectors;
import Math;

using namespace YAML;

namespace Yart::Yaml
{
	export class Config
	{
	public:
		unsigned int Iterations{};
        Vector2 ColorClamp{};
	};

    export std::shared_ptr<Config> ParseConfigNode(const Node& node)
    {
        auto config = std::shared_ptr<Config>{new Config{
            .Iterations = node["iterations"].as<unsigned int>(),
            .ColorClamp = node["colorClamp"].as<Vector2>(),
        }};

        return config;
    }
}