module;

#include "yaml-cpp/yaml.h"

export module YamlNodes:Materials;

import <memory>;
import <string>;
import <unordered_map>;

import :Vectors;
import EmissiveMaterial;
import GgxMaterial;
import LambertianMaterial;
import LambertianMaterial;
import Material;
import Math;
import MirrorMaterial;
import Vector3;

using namespace YAML;

namespace Yart::Yaml
{
	export using MaterialMap = std::unordered_map<std::string, std::shared_ptr<const Material>>;

	void ParseEmissiveMaterial(const Node& node, MaterialMap& materialMap)
	{
		auto name = node["name"].as<std::string>();
		auto emissiveColor = node["emissiveColor"].as<Vector3>();

		auto material = std::make_shared<EmissiveMaterial>(emissiveColor);
		materialMap[name] = material;
	}

    void ParseLambertianMaterial(const Node& node, MaterialMap& materialMap)
    {
        auto name = node["name"].as<std::string>();
        auto diffuseColor = node["diffuseColor"].as<Vector3>();

        auto material = std::make_shared<LambertianMaterial<false>>(diffuseColor);
        materialMap[name] = material;
    }

    void ParseGgxMaterial(const Node& node, MaterialMap& materialMap)
    {
        auto name = node["name"].as<std::string>();
        auto diffuseColor = node["diffuseColor"].as<Vector3>();
        auto specularColor = node["specularColor"].as<Vector3>();
        float roughness = node["roughness"].as<float>();

        auto material = std::make_shared<GgxMaterial>(diffuseColor, specularColor, roughness);
        materialMap[name] = material;
    }

    void ParseMirrorMaterial(const Node& node, MaterialMap& materialMap)
    {
        auto name = node["name"].as<std::string>();

        auto material = std::make_shared<MirrorMaterial>();
        materialMap[name] = material;
    }

    void ParseMaterialNode(const Node& node, MaterialMap& materialMap)
	{
		auto emissiveMaterialNode = node["emissiveMaterial"];
		auto lambertianMaterialNode = node["lambertianMaterial"];
		auto ggxMaterialNode = node["ggxMaterial"];
		auto mirrorMaterialNode = node["mirrorMaterial"];

		if (emissiveMaterialNode)
		{
            ParseEmissiveMaterial(emissiveMaterialNode, materialMap);
		}
		else if (lambertianMaterialNode)
		{
            ParseLambertianMaterial(lambertianMaterialNode, materialMap);
		}
		else if (ggxMaterialNode)
		{
            ParseGgxMaterial(ggxMaterialNode, materialMap);
		}
		else if (mirrorMaterialNode)
		{
            ParseMirrorMaterial(mirrorMaterialNode, materialMap);
		}
	}

	export std::shared_ptr<MaterialMap> ParseMaterialsNode(const Node& node)
	{
        auto materialMap = std::shared_ptr<MaterialMap>{new MaterialMap{}};

        if (!node.IsSequence())
        {
            return materialMap;
        }

        for (const Node& childNode : node)
        {
            ParseMaterialNode(childNode, *materialMap);
        }

        return materialMap;
	}
}