module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Lights;

import <functional>;
import <memory>;

import :Vectors;
import DirectionalLight;
import Light;
import PointLight;
import Vector3;

using namespace YAML;

namespace Yart::Yaml
{
    std::shared_ptr<const Light> ParseDirectionalLight(const Node& node)
    {
        auto color = node["color"].as<Vector3>();
        auto direction = node["direction"].as<Vector3>();

        return std::make_shared<const DirectionalLight>(color, direction);
    }

    std::shared_ptr<const Light> ParsePointLight(const Node& node)
    {
        auto color = node["color"].as<Vector3>();
        auto position = node["position"].as<Vector3>();

        return std::make_shared<const PointLight>(color, position);
    }

    static std::vector<std::tuple<std::string, std::function<std::shared_ptr<const Light>(const Node&)>>> LightMapFunctions
    {
        {"directional", &ParseDirectionalLight},
        {"point", &ParsePointLight},
    };

    std::shared_ptr<const Light> ParseLightNode(const Node& node)
    {
        for (const auto& [nodeName, functionPointer] : LightMapFunctions)
        {
            auto childNode = node[nodeName];
            if (childNode)
            {
                return functionPointer(childNode);
            }
        }
    }

    export std::vector<std::shared_ptr<const Light>> ParseLightsNode(const Node& node)
    {
        std::vector<std::shared_ptr<const Light>> lights{};

        if (!node.IsSequence())
        {
            return lights;
        }

        for (const Node& childNode : node)
        {
            lights.push_back(ParseLightNode(childNode));
        }

        return lights;
    }
}