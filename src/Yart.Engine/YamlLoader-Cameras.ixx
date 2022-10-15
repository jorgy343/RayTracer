module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Cameras;

import <functional>;

import "Common.h";

import :Vectors;
import Camera;
import Math;
import OrthographicCamera;
import PerspectiveCamera;

using namespace YAML;

namespace Yart::Yaml
{
    std::shared_ptr<PerspectiveCamera<double, real>> ParsePerspectiveCamera(const Node& node)
    {
        DoubleVector3 position = ParseVector3<double>(node["position"]);
        DoubleVector3 lookAt = ParseVector3<double>(node["lookAt"]);
        DoubleVector3 up = ParseVector3<double>(node["up"]);
        double fov = Math::deg_to_rad(node["fov"].as<double>());
        UIntVector2 screenSize = ParseVector2<unsigned int>(node["screenSize"]);
        unsigned int subpixelCount = node["subpixelCount"].as<unsigned int>();

        return std::make_unique<PerspectiveCamera<double, real>>(position, lookAt, up, subpixelCount, screenSize, fov);
    }

    std::shared_ptr<OrthographicCamera<double, real>> ParseOrthographicCamera(const Node& node)
    {
        DoubleVector3 position = ParseVector3<double>(node["position"]);
        DoubleVector3 lookAt = ParseVector3<double>(node["lookAt"]);
        DoubleVector3 up = ParseVector3<double>(node["up"]);
        DoubleVector2 orthoSize = ParseVector2<double>(node["orthoSize"]);
        UIntVector2 screenSize = ParseVector2<unsigned int>(node["screenSize"]);
        unsigned int subpixelCount = node["subpixelCount"].as<unsigned int>();

        return std::make_unique<OrthographicCamera<double, real>>(position, lookAt, up, subpixelCount, screenSize, orthoSize);
    }

    static std::vector<std::tuple<std::string, std::function<std::shared_ptr<Camera>(const Node&)>>> CameraMapFunctions
    {
        {"perspective", &ParsePerspectiveCamera},
        {"orthographic", &ParseOrthographicCamera},
    };

    export std::shared_ptr<Camera> ParseCameraNode(const Node& node)
    {
        for (const auto& [nodeName, functionPointer] : CameraMapFunctions)
        {
            auto childNode = node[nodeName];
            if (childNode)
            {
                auto camera = functionPointer(childNode);
                return camera;
            }
        }

        return {};
    }
}