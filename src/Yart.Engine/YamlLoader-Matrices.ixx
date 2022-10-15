module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Matrices;

import "Common.h";

import :Vectors;
import Math;

using namespace Yart;
using namespace YAML;

namespace Yart::Yaml
{
    template <real_number T>
    Matrix4x4T<T> ParseTransformationSequence(const Node& node);

    template<any_number T = real>
    Matrix3x3T<T> ParseMatrix3x3(const Node& node)
    {
        if (node.IsSequence())
        {
            if (node.size() == 1)
            {
                return
                {
                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),

                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),

                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),
                };
            }
            else if (node.size() == 9)
            {
                return
                {
                    node[0].as<T>(),
                    node[1].as<T>(),
                    node[2].as<T>(),

                    node[3].as<T>(),
                    node[4].as<T>(),
                    node[5].as<T>(),

                    node[6].as<T>(),
                    node[7].as<T>(),
                    node[8].as<T>(),
                };
            }
        }

        return Matrix3x3T<T>{};
    }

    template<any_number T = real>
    Matrix4x4T<T> ParseMatrix4x4(const Node& node)
    {
        if (node.IsSequence())
        {
            if (node.size() == 1)
            {
                return
                {
                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),

                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),

                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),

                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),
                    node[0].as<T>(),
                };
            }
            else if (node.size() == 9)
            {
                return
                {
                    node[0].as<T>(),
                    node[1].as<T>(),
                    node[2].as<T>(),
                    node[3].as<T>(),

                    node[4].as<T>(),
                    node[5].as<T>(),
                    node[6].as<T>(),
                    node[7].as<T>(),

                    node[8].as<T>(),
                    node[9].as<T>(),
                    node[10].as<T>(),
                    node[11].as<T>(),

                    node[12].as<T>(),
                    node[13].as<T>(),
                    node[14].as<T>(),
                    node[15].as<T>(),
                };
            }
        }
        else
        {
            auto buildNode = node["build"];

            if (buildNode.IsDefined())
            {
                return ParseTransformationSequence<T>(buildNode);
            }
        }

        return Matrix4x4T<T>{};
    }

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
                result *= ParseMatrix4x4<T>(matrixNode);
            }
            else if (scaleNode)
            {
                auto scale = ParseVector3<T>(scaleNode);
                result *= Matrix4x4T<T>::CreateScale(scale);
            }
            else if (rotateNode)
            {
                auto rotation = ParseVector3<T>(rotateNode);
                result *= Matrix4x4T<T>::CreateRotation(Vector3T<T>{Math::deg_to_rad(rotation.X), Math::deg_to_rad(rotation.Y), Math::deg_to_rad(rotation.Z)});
            }
            else if (translateNode)
            {
                auto translate = ParseVector3<T>(translateNode);
                result *= Matrix4x4T<T>::CreateTranslation(translate);
            }
        }

        return result;
    }
}