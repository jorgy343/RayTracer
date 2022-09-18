module;

#include "yaml-cpp/yaml.h"

export module YamlNodes:Config;

import Vector2;

namespace Yart::Yaml
{
	export class Config
	{
	public:
		UIntVector2 ScreenSize{};
		unsigned int SubpixelCount{};
		unsigned int Iterations{};
	};
}

using namespace Yart::Yaml;

namespace YAML
{
	export template<>
	struct convert<Config>
	{
		static Node encode(const Config& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, Config& rhs)
		{
			rhs.ScreenSize = {5, 10};
			rhs.SubpixelCount = 4;
			rhs.Iterations = 8;

			return true;
		}
	};
}