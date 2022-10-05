module;

#include "yaml-cpp/yaml.h"

#include "Vcl.h"

export module YamlLoader:Matrices;

import "Common.h";

import :Vectors;
import Math;

using namespace Yart;

namespace YAML
{
    export template<>
        struct convert<FloatMatrix3x3>
    {
        static Node encode(const FloatMatrix3x3& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, FloatMatrix3x3& rhs)
        {
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.M11 = node[0].as<float>();
                rhs.M12 = node[0].as<float>();
                rhs.M13 = node[0].as<float>();

                rhs.M21 = node[0].as<float>();
                rhs.M22 = node[0].as<float>();
                rhs.M23 = node[0].as<float>();

                rhs.M31 = node[0].as<float>();
                rhs.M32 = node[0].as<float>();
                rhs.M33 = node[0].as<float>();
            }
            else if (node.size() == 9)
            {
                rhs.M11 = node[0].as<float>();
                rhs.M12 = node[1].as<float>();
                rhs.M13 = node[2].as<float>();

                rhs.M21 = node[3].as<float>();
                rhs.M22 = node[4].as<float>();
                rhs.M23 = node[5].as<float>();

                rhs.M31 = node[6].as<float>();
                rhs.M32 = node[7].as<float>();
                rhs.M33 = node[8].as<float>();
            }
            else
            {
                return false;
            }

            return true;
        }
    };

    export template<>
        struct convert<DoubleMatrix3x3>
    {
        static Node encode(const DoubleMatrix3x3& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, DoubleMatrix3x3& rhs)
        {
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.M11 = node[0].as<double>();
                rhs.M12 = node[0].as<double>();
                rhs.M13 = node[0].as<double>();

                rhs.M21 = node[0].as<double>();
                rhs.M22 = node[0].as<double>();
                rhs.M23 = node[0].as<double>();

                rhs.M31 = node[0].as<double>();
                rhs.M32 = node[0].as<double>();
                rhs.M33 = node[0].as<double>();
            }
            else if (node.size() == 9)
            {
                rhs.M11 = node[0].as<double>();
                rhs.M12 = node[1].as<double>();
                rhs.M13 = node[2].as<double>();

                rhs.M21 = node[3].as<double>();
                rhs.M22 = node[4].as<double>();
                rhs.M23 = node[5].as<double>();

                rhs.M31 = node[6].as<double>();
                rhs.M32 = node[7].as<double>();
                rhs.M33 = node[8].as<double>();
            }
            else
            {
                return false;
            }

            return true;
        }
    };

    export template<>
        struct convert<FloatMatrix4x4>
    {
        static Node encode(const FloatMatrix4x4& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, FloatMatrix4x4& rhs)
        {
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.M11 = node[0].as<float>();
                rhs.M12 = node[0].as<float>();
                rhs.M13 = node[0].as<float>();
                rhs.M14 = node[0].as<float>();

                rhs.M21 = node[0].as<float>();
                rhs.M22 = node[0].as<float>();
                rhs.M23 = node[0].as<float>();
                rhs.M24 = node[0].as<float>();

                rhs.M31 = node[0].as<float>();
                rhs.M32 = node[0].as<float>();
                rhs.M33 = node[0].as<float>();
                rhs.M34 = node[0].as<float>();

                rhs.M41 = node[0].as<float>();
                rhs.M42 = node[0].as<float>();
                rhs.M43 = node[0].as<float>();
                rhs.M44 = node[0].as<float>();
            }
            else if (node.size() == 16)
            {
                rhs.M11 = node[0].as<float>();
                rhs.M12 = node[1].as<float>();
                rhs.M13 = node[2].as<float>();
                rhs.M14 = node[3].as<float>();

                rhs.M21 = node[4].as<float>();
                rhs.M22 = node[5].as<float>();
                rhs.M23 = node[6].as<float>();
                rhs.M24 = node[7].as<float>();

                rhs.M31 = node[8].as<float>();
                rhs.M32 = node[9].as<float>();
                rhs.M33 = node[10].as<float>();
                rhs.M34 = node[11].as<float>();

                rhs.M41 = node[12].as<float>();
                rhs.M42 = node[13].as<float>();
                rhs.M43 = node[14].as<float>();
                rhs.M44 = node[15].as<float>();
            }
            else
            {
                return false;
            }

            return true;
        }
    };

    export template<>
        struct convert<DoubleMatrix4x4>
    {
        static Node encode(const DoubleMatrix4x4& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, DoubleMatrix4x4& rhs)
        {
            if (!node.IsSequence())
            {
                return false;
            }

            if (node.size() == 1)
            {
                rhs.M11 = node[0].as<double>();
                rhs.M12 = node[0].as<double>();
                rhs.M13 = node[0].as<double>();
                rhs.M14 = node[0].as<double>();

                rhs.M21 = node[0].as<double>();
                rhs.M22 = node[0].as<double>();
                rhs.M23 = node[0].as<double>();
                rhs.M24 = node[0].as<double>();

                rhs.M31 = node[0].as<double>();
                rhs.M32 = node[0].as<double>();
                rhs.M33 = node[0].as<double>();
                rhs.M34 = node[0].as<double>();

                rhs.M41 = node[0].as<double>();
                rhs.M42 = node[0].as<double>();
                rhs.M43 = node[0].as<double>();
                rhs.M44 = node[0].as<double>();
            }
            else if (node.size() == 16)
            {
                rhs.M11 = node[0].as<double>();
                rhs.M12 = node[1].as<double>();
                rhs.M13 = node[2].as<double>();
                rhs.M14 = node[3].as<double>();

                rhs.M21 = node[4].as<double>();
                rhs.M22 = node[5].as<double>();
                rhs.M23 = node[6].as<double>();
                rhs.M24 = node[7].as<double>();

                rhs.M31 = node[8].as<double>();
                rhs.M32 = node[9].as<double>();
                rhs.M33 = node[10].as<double>();
                rhs.M34 = node[11].as<double>();

                rhs.M41 = node[12].as<double>();
                rhs.M42 = node[13].as<double>();
                rhs.M43 = node[14].as<double>();
                rhs.M44 = node[15].as<double>();
            }
            else
            {
                return false;
            }

            return true;
        }
    };
}

using namespace YAML;

namespace Yart::Yaml
{
    template <real_number T>
    Matrix4x4T<T> ParseTransformationSequence(const Node& node)
    {
        if (!node.IsSequence())
        {
            return Matrix4x4T<T>{};
        }

        Matrix4x4T<T> result = Matrix4x4T<T>::CreateIdentity();
        for (const Node& childNode : node)
        {
            const Node& matrixNode = childNode["matrix"];
            const Node& scaleNode = childNode["scale"];
            const Node& rotateNode = childNode["rotate"];
            const Node& translateNode = childNode["translate"];

            if (matrixNode)
            {
                result *= matrixNode.as<Matrix4x4T<T>>();
            }
            else if (scaleNode)
            {
                auto scale = scaleNode.as<Vector3T<T>>();
                result *= Matrix4x4T<T>::CreateScale(scale);
            }
            else if (rotateNode)
            {
                auto rotation = rotateNode.as<Vector3T<T>>();
                result *= Matrix4x4T<T>::CreateRotation(Vector3T<T>{Math::deg_to_rad(rotation.X), Math::deg_to_rad(rotation.Y), Math::deg_to_rad(rotation.Z)});
            }
            else if (translateNode)
            {
                auto translate = translateNode.as<Vector3T<T>>();
                result *= Matrix4x4T<T>::CreateTranslation(translate);
            }
        }

        return result;
    }
}