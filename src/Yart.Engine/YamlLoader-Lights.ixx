module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Lights;

import <functional>;

import "Common.h";

import :Vectors;
import DirectionalLight;
import Light;
import Math;
import PointLight;

using namespace YAML;

namespace Yart::Yaml
{
    std::shared_ptr<const Light> ParseDirectionalLight(const Node& node)
    {
        auto color = ParseColor3(node["color"]);
        auto direction = ParseVector3(node["direction"]);

        return std::make_shared<const DirectionalLight>(color, direction);
    }

    std::shared_ptr<const Light> ParsePointLight(const Node& node)
    {
        auto color = ParseColor3(node["color"]);
        auto position = ParseVector3(node["position"]);

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

        return {};
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