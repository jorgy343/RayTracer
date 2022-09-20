module;

#include "yaml-cpp/yaml.h"

export module YamlNodes:Scene;

import <memory>;
import <unordered_map>;

import :Materials;
import :Vectors;
import AreaLight;
import IntersectableGeometry;
import Material;
import Parallelogram;
import Plane;
import Sphere;
import Vector3;

using namespace YAML;

namespace Yart::Yaml
{
	export class SceneConfig
	{
	public:
		std::vector<std::unique_ptr<AreaLight>> AreaLights{};
		std::vector<std::shared_ptr<IntersectableGeometry>> Geometries{};
	};

	std::unique_ptr<Sphere> ParseSphereNode(const Node& node, const MaterialMap& materialMap)
	{
		auto materialName = node["material"].as<std::string>();
		auto material = materialMap.find(materialName)->second;

		auto position = node["position"].as<Vector3>();
		auto radius = node["radius"].as<float>();

		return std::make_unique<Sphere>(position, radius, material.get());
	}

	std::unique_ptr<Plane> ParsePlaneNode(const Node& node, const MaterialMap& materialMap)
	{
		auto materialName = node["material"].as<std::string>();
		auto material = materialMap.find(materialName)->second;

		auto normal = node["normal"].as<Vector3>();
		auto point = node["point"].as<Vector3>();

		return std::make_unique<Plane>(normal, point, material.get());
	}

	std::unique_ptr<Parallelogram> ParseParallelogramNode(const Node& node, const MaterialMap& materialMap)
	{
		auto materialName = node["material"].as<std::string>();
		auto material = materialMap.find(materialName)->second;

		auto position = node["position"].as<Vector3>();
		auto edge1 = node["edge1"].as<Vector3>();
		auto edge2 = node["edge2"].as<Vector3>();

		return std::make_unique<Parallelogram>(position, edge1, edge2, material.get());
	}

	std::unique_ptr<AreaLight> ParseAreaLightNode(const Node& node, const MaterialMap& materialMap)
	{
		auto parallelogramNode = node["parallelogram"];

		if (parallelogramNode)
		{
			return std::move(ParseParallelogramNode(parallelogramNode, materialMap));
		}
        else
        {
            return std::unique_ptr<AreaLight>{};
		}
	}

	std::unique_ptr<IntersectableGeometry> ParseGeometryNode(const Node& node, const MaterialMap& materialMap)
	{
		auto sphereNode = node["sphere"];
		auto planeNode = node["plane"];
		auto parallelogramNode = node["parallelogram"];

		if (sphereNode)
		{
			return std::move(ParseSphereNode(sphereNode, materialMap));
		}
		else if (planeNode)
		{
			return std::move(ParseParallelogramNode(planeNode, materialMap));
		}
		else if (parallelogramNode)
		{
			return std::move(ParseParallelogramNode(parallelogramNode, materialMap));
		}
		else
		{
            return std::unique_ptr<IntersectableGeometry>{};
		}
	}

	export SceneConfig ParseSceneNode(const Node& node, const MaterialMap& materialMap)
	{
		SceneConfig config{};

		for (const Node& childNode : node["areaLights"])
		{
			auto areaLight = ParseAreaLightNode(childNode, materialMap);
			config.AreaLights.push_back(std::move(areaLight));
		}

		for (const Node& childNode : node["geometries"])
		{
			auto geometry = ParseGeometryNode(childNode, materialMap);
			config.Geometries.push_back(std::move(geometry));
		}

		return config;
	}
}