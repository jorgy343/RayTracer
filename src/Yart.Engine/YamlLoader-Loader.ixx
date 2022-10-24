module;

#include "yaml-cpp/yaml.h"

export module YamlLoader:Loader;

import "Common.h";

import :Cameras;
import :Config;
import :Geometry;
import :Lights;
import :Materials;
import :MissShaders;
import Camera;
import IntersectableGeometry;
import Light;
import Material;
import MissShader;

using namespace YAML;

namespace Yart::Yaml
{
    export class YamlData
    {
    public:
        std::shared_ptr<Config> Config{};
        std::shared_ptr<Camera> Camera{};
        std::shared_ptr<MissShader> MissShader{};
        std::vector<std::unique_ptr<const Material>> Materials{};
        std::vector<std::shared_ptr<const Light>> Lights{};
        std::shared_ptr<ParseGeometryResults> GeometryData{};
    };
    
    export std::shared_ptr<YamlData> LoadYaml()
    {
        Node node = LoadFile("../../../../Yart.Engine/dice.yaml");

        std::vector<std::unique_ptr<const Material>> materials{};

        std::shared_ptr<Config> config = ParseConfigNode(node["config"]);
        std::shared_ptr<Camera> camera = ParseCameraNode(node["camera"]);
        std::shared_ptr<MissShader> missShader = ParseMissShaderNode(node["missShader"]);
        std::unique_ptr<MaterialMap> materialMap = ParseMaterialsNode(node["materials"], materials);
        std::vector<std::shared_ptr<const Light>> lights = ParseLightsNode(node["lights"]);
        std::shared_ptr<ParseGeometryResults> geometryDataPointer = ParseSceneNode(node["geometry"], *materialMap);

        return std::make_shared<YamlData>(
            config,
            camera,
            missShader,
            std::move(materials),
            lights,
            geometryDataPointer);
    }
}