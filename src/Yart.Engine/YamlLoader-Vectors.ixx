module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Vectors;

import "Common.h";

import Math;

using namespace Yart;

namespace YAML
{
	export template<>
		struct convert<FloatVector2>
	{
		static Node encode(const FloatVector2& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, FloatVector2& rhs)
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
        struct convert<DoubleVector2>
    {
        static Node encode(const DoubleVector2& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, DoubleVector2& rhs)
        {
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.X = node[0].as<double>();
                rhs.Y = node[0].as<double>();
            }
            else if (node.size() == 2)
            {
                rhs.X = node[0].as<double>();
                rhs.Y = node[1].as<double>();
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
		struct convert<FloatVector3>
	{
		static Node encode(const FloatVector3& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, FloatVector3& rhs)
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
        struct convert<DoubleVector3>
    {
        static Node encode(const DoubleVector3& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, DoubleVector3& rhs)
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
		struct convert<FloatVector4>
	{
		static Node encode(const FloatVector4& rhs)
		{
			return {};
		}

		static bool decode(const Node& node, FloatVector4& rhs)
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
        struct convert<DoubleVector4>
    {
        static Node encode(const DoubleVector4& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, DoubleVector4& rhs)
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
                rhs.W = node[0].as<double>();
            }
            else if (node.size() == 4)
            {
                rhs.X = node[0].as<double>();
                rhs.Y = node[1].as<double>();
                rhs.Z = node[2].as<double>();
                rhs.W = node[3].as<double>();
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