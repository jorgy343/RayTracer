module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Config;

import <memory>;

import :Vectors;
import Vector2;
import Vector3;

using namespace YAML;

namespace Yart::Yaml
{
	export class Config
	{
	public:
		unsigned int Iterations{};
        Vector3 BackgroundColor{};
        Vector2 ColorClamp{};
	};

    export std::shared_ptr<Config> ParseConfigNode(const Node& node)
    {
        auto config = std::shared_ptr<Config>{new Config{
            .Iterations = node["iterations"].as<unsigned int>(),
            .BackgroundColor = node["backgroundColor"].as<Vector3>(),
            .ColorClamp = node["colorClamp"].as<Vector2>(),
        }};

        return config;
    }
}