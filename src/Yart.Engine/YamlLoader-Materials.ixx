module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Materials;

import "Common.h";

import <functional>;
import <unordered_map>;

import :Vectors;
import EmissiveMaterial;
import GgxMaterial;
import LambertianMaterial;
import LambertianMaterial;
import Material;
import Math;
import PhongMaterial;
import ReflectiveMaterial;
import RefractiveMaterial;

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

    void ParseReflectiveMaterial(const Node& node, MaterialMap& materialMap)
    {
        auto name = node["name"].as<std::string>();

        auto material = std::make_shared<ReflectiveMaterial>();
        materialMap[name] = material;
    }

    void ParseRefractiveMaterial(const Node& node, MaterialMap& materialMap)
    {
        auto name = node["name"].as<std::string>();
        auto refractionIndex = node["refractionIndex"].as<float>();

        auto material = std::make_shared<RefractiveMaterial>(refractionIndex);
        materialMap[name] = material;
    }

    void ParsePhongMaterial(const Node& node, MaterialMap& materialMap)
    {
        auto name = node["name"].as<std::string>();

        auto ambientColor = node["ambientColor"].as<Vector3>();
        auto diffuseColor = node["diffuseColor"].as<Vector3>();
        auto specularColor = node["specularColor"].as<Vector3>();

        auto shininess = node["shininess"].as<float>();

        auto material = std::make_shared<PhongMaterial>(
            ambientColor,
            diffuseColor,
            specularColor,
            shininess);

        materialMap[name] = material;
    }

    static std::vector<std::tuple<std::string, std::function<void(const Node&, MaterialMap&)>>> MaterialMapFunctions
    {
        {"emissive", &ParseEmissiveMaterial},
        {"lambertian", &ParseLambertianMaterial},
        {"ggx", &ParseGgxMaterial},
        {"reflective", &ParseReflectiveMaterial},
        {"refractive", &ParseRefractiveMaterial},
        {"phong", &ParsePhongMaterial},
    };

    void ParseMaterialNode(const Node& node, MaterialMap& materialMap)
    {
        for (const auto& [nodeName, functionPointer] : MaterialMapFunctions)
        {
            auto childNode = node[nodeName];
            if (childNode)
            {
                functionPointer(childNode, materialMap);
                return;
            }
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