module;

#include "yaml-cpp/yaml.h"

export module YamlNodes:Config;

import <memory>;

using namespace YAML;

namespace Yart::Yaml
{
	export class Config
	{
	public:
		unsigned int Iterations{};
	};

    export std::shared_ptr<Config> ParseConfigNode(const Node& node)
    {
        auto config = std::shared_ptr<Config>{new Config{
            .Iterations = node["iterations"].as<unsigned int>(),
        }};

        return config;
    }
}