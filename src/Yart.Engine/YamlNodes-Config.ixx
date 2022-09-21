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
        unsigned int SubpixelCount{};
	};

    export std::shared_ptr<Config> ParseConfigNode(const Node& node)
    {
        auto config = std::shared_ptr<Config>{new Config{}};

        config->Iterations = node["iterations"].as<unsigned int>();
        config->SubpixelCount = node["subpixelCount"].as<unsigned int>();

        return config;
    }
}