module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Vectors;

import "Common.h";

import Math;

using namespace Yart;
using namespace YAML;

namespace Yart::Yaml
{
    export template<any_number T = real>
        Vector2T<T> ParseVector2(const Node& node)
    {
        if (node.IsSequence())
        {
            if (node.size() == 1)
            {
                return
                {
                    node[0].as<T>(),
                    node[0].as<T>(),
                };
            }
            else if (node.size() == 2)
            {
                return
                {
                    node[0].as<T>(),
                    node[1].as<T>(),
                };
            }
        }
        else
        {
            auto normalizeNode = node["normalize"];

            if (normalizeNode.IsDefined())
            {
                return ParseVector2<T>(normalizeNode).Normalize();
            }
        }

        return Vector2T<T>{};
    }

    export template<any_number T = real>
        Vector3T<T> ParseVector3(const Node& node)
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
                };
            }
            else if (node.size() == 3)
            {
                return
                {
                    node[0].as<T>(),
                    node[1].as<T>(),
                    node[2].as<T>(),
                };
            }
        }
        else
        {
            auto normalizeNode = node["normalize"];

            if (normalizeNode.IsDefined())
            {
                return ParseVector3<T>(normalizeNode).Normalize();
            }
        }

        return Vector3T<T>{};
    }

    export template<any_number T = real>
        Vector4T<T> ParseVector4(const Node& node)
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
                };
            }
            else if (node.size() == 4)
            {
                return
                {
                    node[0].as<T>(),
                    node[1].as<T>(),
                    node[2].as<T>(),
                    node[3].as<T>(),
                };
            }
        }
        else
        {
            auto normalizeNode = node["normalize"];

            if (normalizeNode.IsDefined())
            {
                return ParseVector4<T>(normalizeNode).Normalize();
            }
        }

        return Vector4T<T>{};
    }
}