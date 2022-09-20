module;

#include "yaml-cpp/yaml.h"

export module YamlNodes:Cameras;

import <memory>;

import :Vectors;
import Camera;
import Math;
import PerspectiveCamera;
import Vector3;

using namespace YAML;

namespace Yart::Yaml
{
	std::unique_ptr<PerspectiveCamera> ParsePerspectiveCamera(const Node& node)
	{
		Vector3 position = node["position"].as<Vector3>();
		Vector3 lookAt = node["lookAt"].as<Vector3>();
		Vector3 up = node["up"].as<Vector3>();
		float fov = Math::deg_to_rad(node["fov"].as<float>());
		UIntVector2 screenSize = node["screenSize"].as<UIntVector2>();
		unsigned int subpixelCount = node["subpixelCount"].as<unsigned int>();

		return std::make_unique<PerspectiveCamera>(position, lookAt, up, subpixelCount, screenSize, fov);
	}

	export std::unique_ptr<Camera> ParseCameraNode(const Node& node)
	{
		auto perspectiveCameraNode = node["perspectiveCamera"];

		if (perspectiveCameraNode)
		{
			return std::move(ParsePerspectiveCamera(perspectiveCameraNode));
		}

        return std::unique_ptr<Camera>{};
	}
}