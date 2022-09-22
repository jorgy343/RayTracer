module;

#include "range/v3/view/chunk.hpp"

#include "yaml-cpp/yaml.h"

export module YamlNodes:Scene;

import <memory>;
import <tuple>;
import <unordered_map>;

import :Materials;
import :Vectors;
import AreaLight;
import AxisAlignedBox;
import AxisAlignedBoxSoa;
import BoundingGeometry;
import GeometryCollection;
import GeometrySoa;
import IntersectableGeometry;
import Material;
import Parallelogram;
import ParallelogramSoa;
import Plane;
import PlaneSoa;
import Sphere;
import SphereSoa;
import Vector3;

using namespace YAML;

namespace Yart::Yaml
{
    export class SceneConfig
    {
    public:
        std::vector<std::shared_ptr<const IntersectableGeometry>> Geometries{};
        std::vector<const AreaLight*> AreaLights{};

        const IntersectableGeometry* Geometry{};
    };

    class SequenceVectors
    {
    public:
        std::vector<const Sphere*> Spheres{};
        std::vector<const Plane*> Planes{};
        std::vector<const Parallelogram*> Parallelograms{};
        std::vector<const AxisAlignedBox*> AsixAlignedBoxes{};
    };

    std::tuple<const IntersectableGeometry*, bool> ParseGeometryNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, SequenceVectors* sequenceVectors);
    std::shared_ptr<std::vector<const IntersectableGeometry*>> ParseGeometrySequenceNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig);

    const Sphere* ParseSphereNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto position = node["position"].as<Vector3>();
        auto radius = node["radius"].as<float>();

        auto geometry = std::make_shared<const Sphere>(position, radius, material);
        sceneConfig.Geometries.push_back(geometry);

        return geometry.get();
    }

    const Plane* ParsePlaneNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto normal = node["normal"].as<Vector3>();
        auto point = node["point"].as<Vector3>();

        auto geometry = std::make_shared<const Plane>(normal, point, material);
        sceneConfig.Geometries.push_back(geometry);

        return geometry.get();
    }

    const Parallelogram* ParseParallelogramNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig)
    {
        auto areaLight = node["areaLight"].as<bool>(false);

        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto position = node["position"].as<Vector3>();
        auto edge1 = node["edge1"].as<Vector3>();
        auto edge2 = node["edge2"].as<Vector3>();

        auto geometry = std::make_shared<const Parallelogram>(position, edge1, edge2, material);
        sceneConfig.Geometries.push_back(geometry);

        if (areaLight)
        {
            sceneConfig.AreaLights.push_back(geometry.get());
        }

        return geometry.get();
    }

    const AxisAlignedBox* ParseAxisAlignedBoxNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto minimum = node["minimum"].as<Vector3>();
        auto maximum = node["maximum"].as<Vector3>();

        auto geometry = std::make_shared<const AxisAlignedBox>(minimum, maximum, material);
        sceneConfig.Geometries.push_back(geometry);

        return geometry.get();
    }

    const GeometryCollection* ParseGeometryCollectionNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig)
    {
        auto children = ParseGeometrySequenceNode(node["children"], materialMap, sceneConfig);

        auto geometry = std::make_shared<const GeometryCollection>(*children);
        sceneConfig.Geometries.push_back(geometry);

        return geometry.get();
    }

    const BoundingGeometry* ParseBoundingGeometryNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig)
    {
        auto [boundingVolume, ignored1] = ParseGeometryNode(node["boundingVolume"], materialMap, sceneConfig, nullptr);
        auto [child, ignored2] = ParseGeometryNode(node["child"], materialMap, sceneConfig, nullptr);

        auto geometry = std::make_shared<const BoundingGeometry>(boundingVolume, child);
        sceneConfig.Geometries.push_back(geometry);

        return geometry.get();
    }

    std::tuple<const IntersectableGeometry*, bool> ParseGeometryNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, SequenceVectors* sequenceVectors)
    {
        auto sphereNode = node["sphere"];
        auto planeNode = node["plane"];
        auto parallelogramNode = node["parallelogram"];
        auto axisAlignedBoxNode = node["axisAlignedBox"];
        auto geometryCollectionNode = node["geometryCollection"];
        auto boundingGeometryNode = node["boundingGeometry"];

        if (sphereNode)
        {
            auto geometry = ParseSphereNode(sphereNode, materialMap, sceneConfig);

            if (sequenceVectors)
            {
                sequenceVectors->Spheres.push_back(geometry);
            }

            return std::make_tuple(geometry, false);
        }
        else if (planeNode)
        {
            auto geometry = ParsePlaneNode(planeNode, materialMap, sceneConfig);

            if (sequenceVectors)
            {
                sequenceVectors->Planes.push_back(geometry);
            }

            return std::make_tuple(geometry, false);
        }
        else if (parallelogramNode)
        {
            auto geometry = ParseParallelogramNode(parallelogramNode, materialMap, sceneConfig);

            if (sequenceVectors)
            {
                sequenceVectors->Parallelograms.push_back(geometry);
            }

            return std::make_tuple(geometry, false);
        }
        else if (axisAlignedBoxNode)
        {
            auto geometry = ParseAxisAlignedBoxNode(axisAlignedBoxNode, materialMap, sceneConfig);

            if (sequenceVectors)
            {
                sequenceVectors->AsixAlignedBoxes.push_back(geometry);
            }

            return std::make_tuple(geometry, false);
        }
        else if (geometryCollectionNode)
        {
            auto geometry = ParseGeometryCollectionNode(geometryCollectionNode, materialMap, sceneConfig);
            return std::make_tuple(geometry, true);
        }
        else if (boundingGeometryNode)
        {
            auto geometry = ParseBoundingGeometryNode(boundingGeometryNode, materialMap, sceneConfig);
            return std::make_tuple(geometry, true);
        }

        return std::make_tuple(nullptr, false);
    }

    template <IntersectableGeometryConcept TGeometry, typename TGeometrySoa>
        requires std::derived_from<TGeometrySoa, GeometrySoa<TGeometry>>
    void CreateSoa(std::vector<const TGeometry*>& geometries, SceneConfig& sceneConfig, std::shared_ptr<std::vector<const IntersectableGeometry*>>& returnGeometries)
    {
        auto chunks = geometries | ranges::views::chunk(8);
        for (const auto& chunk : chunks)
        {
            if (chunk.size() == 1)
            {
                returnGeometries->push_back(chunk[0]);
            }
            else
            {
                auto soa = std::shared_ptr<TGeometrySoa>{new TGeometrySoa{}};

                sceneConfig.Geometries.push_back(soa);
                returnGeometries->push_back(soa.get());

                int index = 0;
                for (const auto& geometry : chunk)
                {
                    soa->Insert(index++, geometry);
                }
            }
        }
    }

    std::shared_ptr<std::vector<const IntersectableGeometry*>> ParseGeometrySequenceNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig)
    {
        SequenceVectors sequenceVectors{};
        auto geometries = std::shared_ptr<std::vector<const IntersectableGeometry*>>{new std::vector<const IntersectableGeometry*>{}};

        for (const Node& childNode : node)
        {
            auto [geometry, shouldInclude] = ParseGeometryNode(childNode, materialMap, sceneConfig, &sequenceVectors);
            if (shouldInclude)
            {
                geometries->push_back(geometry);
            }
        }

        CreateSoa<Sphere, SphereSoa>(sequenceVectors.Spheres, sceneConfig, geometries);
        CreateSoa<Plane, PlaneSoa>(sequenceVectors.Planes, sceneConfig, geometries);
        CreateSoa<Parallelogram, ParallelogramSoa>(sequenceVectors.Parallelograms, sceneConfig, geometries);
        CreateSoa<AxisAlignedBox, AxisAlignedBoxSoa>(sequenceVectors.AsixAlignedBoxes, sceneConfig, geometries);

        return geometries;
    }

    export std::shared_ptr<SceneConfig> ParseSceneNode(const Node& node, const MaterialMap& materialMap)
    {
        auto sceneConfig = std::shared_ptr<SceneConfig>(new SceneConfig{});

        auto [geometry, _] = ParseGeometryNode(node["geometry"], materialMap, *sceneConfig, nullptr);
        sceneConfig->Geometry = geometry;

        return sceneConfig;
    }
}