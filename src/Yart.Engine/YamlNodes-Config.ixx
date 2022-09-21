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
        unsigned int SubpixelCount{};
	};

    export Config ParseConfigNode(const Node& node)
    {
        Config config{};

        config.Iterations = node["iterations"].as<unsigned int>();
        config.SubpixelCount = node["subpixelCount"].as<unsigned int>();

        return config;
    }
}