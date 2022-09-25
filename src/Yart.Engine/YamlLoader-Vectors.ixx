module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Vectors;

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
			if (!node.IsSequence())
			{
				return false;
			}

            if (node.size() == 1)
            {
                rhs.X = node[0].as<float>();
                rhs.Y = node[0].as<float>();
            }
            else if (node.size() == 2)
            {
                rhs.X = node[0].as<float>();
                rhs.Y = node[1].as<float>();
            }
            else
            {
                return false;
            }

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
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<int>();
                rhs.Y = node[0].as<int>();
            }
            else if (node.size() == 2)
            {
                rhs.X = node[0].as<int>();
                rhs.Y = node[1].as<int>();
            }
            else
            {
                return false;
            }

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
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<unsigned int>();
                rhs.Y = node[0].as<unsigned int>();
            }
            else if (node.size() == 2)
            {
                rhs.X = node[0].as<unsigned int>();
                rhs.Y = node[1].as<unsigned int>();
            }
            else
            {
                return false;
            }

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
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<float>();
                rhs.Y = node[0].as<float>();
                rhs.Z = node[0].as<float>();
            }
            else if (node.size() == 3)
            {
                rhs.X = node[0].as<float>();
                rhs.Y = node[1].as<float>();
                rhs.Z = node[2].as<float>();
            }
            else
            {
                return false;
            }

			return true;
		}
	};

    export template<>
        struct convert<Vector3T<double>>
    {
        static Node encode(const Vector3T<double>& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, Vector3T<double>& rhs)
        {
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<double>();
                rhs.Y = node[0].as<double>();
                rhs.Z = node[0].as<double>();
            }
            else if (node.size() == 3)
            {
                rhs.X = node[0].as<double>();
                rhs.Y = node[1].as<double>();
                rhs.Z = node[2].as<double>();
            }
            else
            {
                return false;
            }

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
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<int>();
                rhs.Y = node[0].as<int>();
                rhs.Z = node[0].as<int>();
            }
            else if (node.size() == 3)
            {
                rhs.X = node[0].as<int>();
                rhs.Y = node[1].as<int>();
                rhs.Z = node[2].as<int>();
            }
            else
            {
                return false;
            }

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
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<unsigned int>();
                rhs.Y = node[0].as<unsigned int>();
                rhs.Z = node[0].as<unsigned int>();
            }
            else if (node.size() == 3)
            {
                rhs.X = node[0].as<unsigned int>();
                rhs.Y = node[1].as<unsigned int>();
                rhs.Z = node[2].as<unsigned int>();
            }
            else
            {
                return false;
            }

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
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<float>();
                rhs.Y = node[0].as<float>();
                rhs.Z = node[0].as<float>();
                rhs.W = node[0].as<float>();
            }
            else if (node.size() == 4)
            {
                rhs.X = node[0].as<float>();
                rhs.Y = node[1].as<float>();
                rhs.Z = node[2].as<float>();
                rhs.W = node[3].as<float>();
            }
            else
            {
                return false;
            }

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
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<int>();
                rhs.Y = node[0].as<int>();
                rhs.Z = node[0].as<int>();
                rhs.W = node[0].as<int>();
            }
            else if (node.size() == 4)
            {
                rhs.X = node[0].as<int>();
                rhs.Y = node[1].as<int>();
                rhs.Z = node[2].as<int>();
                rhs.W = node[3].as<int>();
            }
            else
            {
                return false;
            }

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
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<unsigned int>();
                rhs.Y = node[0].as<unsigned int>();
                rhs.Z = node[0].as<unsigned int>();
                rhs.W = node[0].as<unsigned int>();
            }
            else if (node.size() == 4)
            {
                rhs.X = node[0].as<unsigned int>();
                rhs.Y = node[1].as<unsigned int>();
                rhs.Z = node[2].as<unsigned int>();
                rhs.W = node[3].as<unsigned int>();
            }
            else
            {
                return false;
            }

			return true;
		}
	};
}