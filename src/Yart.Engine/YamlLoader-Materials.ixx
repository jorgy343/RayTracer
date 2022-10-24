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
import Material;
import Math;
import PhongMaterial;
import ReflectiveMaterial;
import RefractiveMaterial;

using namespace YAML;

namespace Yart::Yaml
{
    export using MaterialMap = std::unordered_map<std::string, const Material*>;

    std::unique_ptr<const EmissiveMaterial> ParseEmissiveMaterial(const Node& node)
    {
        auto emissiveColor = ParseColor3(node["emissiveColor"]);
        return std::make_unique<const EmissiveMaterial>(emissiveColor);
    }

    std::unique_ptr<const LambertianMaterial<false>> ParseLambertianMaterial(const Node& node)
    {
        auto diffuseColor = ParseColor3(node["diffuseColor"]);
        return std::make_unique<const LambertianMaterial<false>>(diffuseColor);
    }

    std::unique_ptr<const GgxMaterial> ParseGgxMaterial(const Node& node)
    {
        auto diffuseColor = ParseColor3(node["diffuseColor"]);
        auto specularColor = ParseColor3(node["specularColor"]);
        auto roughness = node["roughness"].as<real>();

        return std::make_unique<const GgxMaterial>(diffuseColor, specularColor, roughness);
    }

    std::unique_ptr<const ReflectiveMaterial> ParseReflectiveMaterial(const Node& node)
    {
        return std::make_unique<const ReflectiveMaterial>();
    }

    std::unique_ptr<const RefractiveMaterial> ParseRefractiveMaterial(const Node& node)
    {
        auto refractionIndex = node["refractionIndex"].as<real>();
        return std::make_unique<const RefractiveMaterial>(refractionIndex);
    }

    std::unique_ptr<const PhongMaterial> ParsePhongMaterial(const Node& node)
    {
        auto ambientColor = ParseColor3(node["ambientColor"]);
        auto diffuseColor = ParseColor3(node["diffuseColor"]);
        auto specularColor = ParseColor3(node["specularColor"]);

        auto shininess = node["shininess"].as<real>();

        return std::make_unique<const PhongMaterial>(
            ambientColor,
            diffuseColor,
            specularColor,
            shininess);
    }

    static std::vector<std::tuple<std::string, std::function<std::unique_ptr<const Material>(const Node&)>>> ParseMaterialFunctions
    {
        {"emissive", &ParseEmissiveMaterial},
        {"lambertian", &ParseLambertianMaterial},
        {"ggx", &ParseGgxMaterial},
        {"reflective", &ParseReflectiveMaterial},
        {"refractive", &ParseRefractiveMaterial},
        {"phong", &ParsePhongMaterial},
    };

    void ParseMaterialNode(const Node& node, MaterialMap& materialMap, std::vector<std::unique_ptr<const Material>>& materials)
    {
        for (const auto& [nodeName, functionPointer] : ParseMaterialFunctions)
        {
            auto childNode = node[nodeName];
            if (childNode)
            {
                std::unique_ptr<const Material> material = functionPointer(childNode);
                
                auto name = node["name"].as<std::string>();
                materialMap[name] = material.get();

                materials.push_back(std::move(material));

                return;
            }
        }
    }

    export std::unique_ptr<MaterialMap> ParseMaterialsNode(const Node& node, std::vector<std::unique_ptr<const Material>>& materials)
    {
        auto materialMap = std::unique_ptr<MaterialMap>{new MaterialMap{}};

        if (!node.IsSequence())
        {
            return materialMap;
        }

        for (const Node& childNode : node)
        {
            ParseMaterialNode(childNode, *materialMap, materials);
        }

        return materialMap;
    }
}