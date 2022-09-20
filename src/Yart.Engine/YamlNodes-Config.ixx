module;

#include "yaml-cpp/yaml.h"

export module YamlNodes:Config;

using namespace YAML;

namespace Yart::Yaml
{
	export class Config
	{
	public:
		unsigned int Iterations{};
	};

    export Config ParseConfigNode(const Node& node)
    {
        Config config{};

        config.Iterations = node["iterations"].as<int>();

        return config;
    }
}