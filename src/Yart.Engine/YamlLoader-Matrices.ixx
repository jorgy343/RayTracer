module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Matrices;

import :Vectors;
import Math;
import Matrix3x3;
import Matrix4x4;

using namespace Yart;

namespace YAML
{
    export template<>
        struct convert<Matrix3x3>
    {
        static Node encode(const Matrix3x3& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, Matrix3x3& rhs)
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
        struct convert<Matrix4x4>
    {
        static Node encode(const Matrix4x4& rhs)
        {
            return {};
        }

        static bool decode(const Node& node, Matrix4x4& rhs)
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
}

using namespace YAML;

namespace Yart::Yaml
{
    Matrix4x4 ParseTransformationSequence(const Node& node)
    {
        if (!node.IsSequence())
        {
            return Matrix4x4{};
        }

        Matrix4x4 result = Matrix4x4::CreateIdentity();
        for (const Node& childNode : node)
        {
            const Node& matrixNode = childNode["matrix"];
            const Node& scaleNode = childNode["scale"];
            const Node& rotateNode = childNode["rotate"];
            const Node& translateNode = childNode["translate"];

            if (matrixNode)
            {
                result *= matrixNode.as<Matrix4x4>();
            }
            else if (scaleNode)
            {
                Vector3 scale = scaleNode.as<Vector3>();
                result *= Matrix4x4::CreateScale(scale);
            }
            else if (rotateNode)
            {
                Vector3 rotation = rotateNode.as<Vector3>();
                result *= Matrix4x4::CreateRotation(Vector3{Math::deg_to_rad(rotation.X), Math::deg_to_rad(rotation.Y), Math::deg_to_rad(rotation.Z)});
            }
            else if (translateNode)
            {
                Vector3 translate = translateNode.as<Vector3>();
                result *= Matrix4x4::CreateTranslation(translate);
            }
        }

        return result;
    }
}