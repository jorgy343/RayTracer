module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Cameras;

import <functional>;
import <memory>;
import <string>;
import <tuple>;
import <vector>;

import "Common.h";

import :Vectors;
import Camera;
import Math;
import OrthographicCamera;
import PerspectiveCamera;
import Vector2;
import Vector3;

using namespace YAML;

namespace Yart::Yaml
{
    std::shared_ptr<PerspectiveCamera<double, real>> ParsePerspectiveCamera(const Node& node)
    {
        Vector3T<double> position = node["position"].as<Vector3T<double>>();
        Vector3T<double> lookAt = node["lookAt"].as<Vector3T<double>>();
        Vector3T<double> up = node["up"].as<Vector3T<double>>();
        double fov = Math::deg_to_rad(node["fov"].as<double>());
        UIntVector2 screenSize = node["screenSize"].as<UIntVector2>();
        unsigned int subpixelCount = node["subpixelCount"].as<unsigned int>();

        return std::make_unique<PerspectiveCamera<double, real>>(position, lookAt, up, subpixelCount, screenSize, fov);
    }

    std::shared_ptr<OrthographicCamera<double, real>> ParseOrthographicCamera(const Node& node)
    {
        Vector3T<double> position = node["position"].as<Vector3T<double>>();
        Vector3T<double> lookAt = node["lookAt"].as<Vector3T<double>>();
        Vector3T<double> up = node["up"].as<Vector3T<double>>();
        Vector2T<double> orthoSize = node["orthoSize"].as<Vector2T<double>>();
        UIntVector2 screenSize = node["screenSize"].as<UIntVector2>();
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