module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Cameras;

import <memory>;

import "Common.h";

import :Vectors;
import Camera;
import Math;
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

	export std::shared_ptr<Camera> ParseCameraNode(const Node& node)
	{
		auto perspectiveCameraNode = node["perspectiveCamera"];

		if (perspectiveCameraNode)
		{
			return ParsePerspectiveCamera(perspectiveCameraNode);
		}

        return std::shared_ptr<Camera>{};
	}
}