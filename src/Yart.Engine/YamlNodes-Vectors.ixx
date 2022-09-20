module;

#include "yaml-cpp/yaml.h"

export module YamlNodes:Vectors;

import Vector2;
import Vector3;
import Vector4;

using namespace Yart;

namespace YAML
{
	export template<>
		struct convert<Vector2>
	{
		static Node encode(const Vector2& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, Vector2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.X = node[0].as<float>();
			rhs.Y = node[1].as<float>();

			return true;
		}
	};

	export template<>
		struct convert<IntVector2>
	{
		static Node encode(const IntVector2& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, IntVector2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.X = node[0].as<int>();
			rhs.Y = node[1].as<int>();

			return true;
		}
	};

	export template<>
		struct convert<UIntVector2>
	{
		static Node encode(const UIntVector2& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, UIntVector2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.X = node[0].as<unsigned int>();
			rhs.Y = node[1].as<unsigned int>();

			return true;
		}
	};

	export template<>
		struct convert<Vector3>
	{
		static Node encode(const Vector3& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, Vector3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.X = node[0].as<float>();
			rhs.Y = node[1].as<float>();
			rhs.Z = node[2].as<float>();

			return true;
		}
	};

	export template<>
		struct convert<IntVector3>
	{
		static Node encode(const IntVector3& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, IntVector3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.X = node[0].as<int>();
			rhs.Y = node[1].as<int>();
			rhs.Z = node[2].as<int>();

			return true;
		}
	};

	export template<>
		struct convert<UIntVector3>
	{
		static Node encode(const UIntVector3& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, UIntVector3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.X = node[0].as<unsigned int>();
			rhs.Y = node[1].as<unsigned int>();
			rhs.Z = node[2].as<unsigned int>();

			return true;
		}
	};

	export template<>
		struct convert<Vector4>
	{
		static Node encode(const Vector4& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, Vector4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.X = node[0].as<float>();
			rhs.Y = node[1].as<float>();
			rhs.Z = node[2].as<float>();
			rhs.W = node[3].as<float>();

			return true;
		}
	};

	export template<>
		struct convert<IntVector4>
	{
		static Node encode(const IntVector4& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, IntVector4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.X = node[0].as<int>();
			rhs.Y = node[1].as<int>();
			rhs.Z = node[2].as<int>();
			rhs.W = node[3].as<int>();

			return true;
		}
	};

	export template<>
		struct convert<UIntVector4>
	{
		static Node encode(const UIntVector4& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, UIntVector4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.X = node[0].as<unsigned int>();
			rhs.Y = node[1].as<unsigned int>();
			rhs.Z = node[2].as<unsigned int>();
			rhs.W = node[3].as<unsigned int>();

			return true;
		}
	};
}