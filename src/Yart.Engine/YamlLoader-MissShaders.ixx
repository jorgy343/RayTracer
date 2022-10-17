module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:MissShaders;

import <functional>;

import "Common.h";

import :Vectors;
import Math;
import MissShader;
import ConstantMissShader;
import AtmosphereMissShader;

using namespace YAML;

namespace Yart::Yaml
{
    std::shared_ptr<ConstantMissShader> ParseConstantMissShader(const Node& node)
    {
        Color3 color = ParseColor3(node["color"]);

        return std::make_unique<ConstantMissShader>(color);
    }

    std::shared_ptr<AtmosphereMissShader> ParseAtmosphereMissShader(const Node& node)
    {
        Vector3 offset = ParseVector3<real>(node["offset"]);
        Vector3 sunDirection = ParseVector3<real>(node["sunDirection"]);
        Color3 sunIntensity = ParseColor3<real>(node["sunIntensity"]);
        real lambda = node["lambda"].as<real>();
        real planetRadius = node["planetRadius"].as<real>();
        real atmosphereHeight = node["atmosphereHeight"].as<real>();
        real rayleighScaleHeight = node["rayleighScaleHeight"].as<real>();
        real mieScaleHeight = node["mieScaleHeight"].as<real>();
        real indexOfRefractionAtSeaLevel = node["indexOfRefractionAtSeaLevel"].as<real>();
        real numberDensityOfAirAtSeaLevel = node["numberDensityOfAirAtSeaLevel"].as<real>();
        real numberDensityOfAerosolsAtSeaLevel = node["numberDensityOfAerosolsAtSeaLevel"].as<real>();
        real mieU = node["mieU"].as<real>();

        return std::make_unique<AtmosphereMissShader>(
            offset,
            sunDirection,
            sunIntensity,
            lambda,
            planetRadius,
            atmosphereHeight,
            rayleighScaleHeight,
            mieScaleHeight,
            indexOfRefractionAtSeaLevel,
            numberDensityOfAirAtSeaLevel,
            numberDensityOfAerosolsAtSeaLevel,
            mieU);
    }

    static std::vector<std::tuple<std::string, std::function<std::shared_ptr<MissShader>(const Node&)>>> MissShaderMapFunctions
    {
        {"constant", &ParseConstantMissShader},
        {"atmosphere", &ParseAtmosphereMissShader},
    };

    export std::shared_ptr<MissShader> ParseMissShaderNode(const Node& node)
    {
        for (const auto& [nodeName, functionPointer] : MissShaderMapFunctions)
        {
            auto childNode = node[nodeName];
            if (childNode)
            {
                auto missShader = functionPointer(childNode);
                return missShader;
            }
        }

        return {};
    }
}