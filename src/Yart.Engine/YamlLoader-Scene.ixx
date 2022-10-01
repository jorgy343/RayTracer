module;

#include "range/v3/view/chunk.hpp"

#include "yaml-cpp/yaml.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Vcl.h"

export module YamlLoader:Scene;

import <functional>;
import <memory>;
import <string>;
import <tuple>;
import <unordered_map>;
import <vector>;

import :Materials;
import :Matrices;
import :Vectors;
import AreaLight;
import AxisAlignedBox;
import AxisAlignedBoxSoa;
import BoundingBoxHierarchy;
import BoundingGeometry;
import Cylinder;
import Disc;
import GeometryCollection;
import GeometrySoa;
import GeometrySoaUtilities;
import IntersectableGeometry;
import Material;
import Parallelogram;
import ParallelogramSoa;
import Plane;
import PlaneSoa;
import Sphere;
import SphereSoa;
import TransformedGeometry;
import Triangle;
import TriangleSoa;
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

    std::tuple<const IntersectableGeometry*, bool> ParseGeometryNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries, bool geometryOnly);
    std::shared_ptr<std::vector<const IntersectableGeometry*>> ParseGeometrySequenceNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig);

    const Sphere* ParseSphereNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto position = node["position"].as<Vector3>();
        auto radius = node["radius"].as<float>();

        auto geometry = std::make_shared<const Sphere>(position, radius, material);
        sceneConfig.Geometries.push_back(geometry);

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const Plane* ParsePlaneNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto normal = node["normal"].as<Vector3>();
        auto point = node["point"].as<Vector3>();

        auto geometry = std::make_shared<const Plane>(normal, point, material);
        sceneConfig.Geometries.push_back(geometry);

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const Parallelogram* ParseParallelogramNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
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

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const Triangle* ParseTriangleNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto vertex0 = node["vertex0"].as<Vector3>();
        auto vertex1 = node["vertex1"].as<Vector3>();
        auto vertex2 = node["vertex2"].as<Vector3>();

        std::shared_ptr<const Triangle> geometry;

        auto normalNode0 = node["normal0"];
        auto normalNode1 = node["normal1"];
        auto normalNode2 = node["normal2"];

        if (normalNode0 && normalNode1 && normalNode2)
        {
            auto normal0 = normalNode0.as<Vector3>();
            auto normal1 = normalNode1.as<Vector3>();
            auto normal2 = normalNode2.as<Vector3>();

            geometry = std::make_shared<const Triangle>(vertex0, vertex1, vertex2, normal0, normal1, normal2, material);
        }
        else
        {
            geometry = std::make_shared<const Triangle>(vertex0, vertex1, vertex2, material);
        }

        sceneConfig.Geometries.push_back(geometry);

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const Disc* ParseDiscNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto areaLight = node["areaLight"].as<bool>(false);

        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto position = node["position"].as<Vector3>();
        auto normal = node["normal"].as<Vector3>();
        auto radius = node["radius"].as<float>();

        auto geometry = std::make_shared<const Disc>(position, normal, radius, material);
        sceneConfig.Geometries.push_back(geometry);

        if (areaLight)
        {
            sceneConfig.AreaLights.push_back(geometry.get());
        }

        return geometry.get();
    }

    const AxisAlignedBox* ParseAxisAlignedBoxNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto minimum = node["minimum"].as<Vector3>();
        auto maximum = node["maximum"].as<Vector3>();

        auto geometry = std::make_shared<const AxisAlignedBox>(minimum, maximum, material);
        sceneConfig.Geometries.push_back(geometry);

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const Cylinder* ParseCylinderNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto start = node["start"].as<Vector3>();
        auto end = node["end"].as<Vector3>();
        auto radius = node["radius"].as<float>();

        auto geometry = std::make_shared<const Cylinder>(start, end, radius, material);
        sceneConfig.Geometries.push_back(geometry);

        return geometry.get();
    }

    const GeometryCollection* ParseGeometryCollectionNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto children = ParseGeometrySequenceNode(node["children"], materialMap, sceneConfig);

        auto geometry = std::make_shared<const GeometryCollection>(*children);
        sceneConfig.Geometries.push_back(geometry);

        return geometry.get();
    }

    const BoundingGeometry* ParseBoundingGeometryNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto [boundingVolume, ignored1] = ParseGeometryNode(node["boundingVolume"], materialMap, sceneConfig, nullptr, false);
        auto [child, ignored2] = ParseGeometryNode(node["child"], materialMap, sceneConfig, nullptr, false);

        auto geometry = std::make_shared<const BoundingGeometry>(boundingVolume, child);
        sceneConfig.Geometries.push_back(geometry);

        return geometry.get();
    }

    const TransformedGeometry* ParseTransformedGeometryNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto [childGeometry, ignored] = ParseGeometryNode(node["child"], materialMap, sceneConfig, nullptr, true);
        auto matrix = ParseTransformationSequence(node["transformation"]);

        auto geometry = std::make_shared<const TransformedGeometry>(reinterpret_cast<const Geometry*>(childGeometry), matrix);
        sceneConfig.Geometries.push_back(geometry);

        return geometry.get();
    }

    const BoundingGeometry* ParseTriangleMeshObjNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName).get();

        auto transformation = Matrix4x4::CreateIdentity();

        auto transformationNode = node["transformation"];
        if (transformationNode)
        {
            transformation = ParseTransformationSequence(node["transformation"]);
        }

        // Read the geometry from the obj file.
        tinyobj::ObjReader reader{};
        reader.ParseFromFile("../../../../Yart.Engine/teapot.obj"); // TODO: Pull this from the yaml file.

        auto& attributes = reader.GetAttrib();
        auto& shapes = reader.GetShapes();

        BoundingBox meshBoundingBox{
            Vector3{std::numeric_limits<float>::infinity()},
            Vector3{-std::numeric_limits<float>::infinity()},
        };

        std::vector<const IntersectableGeometry*> triangles{};
        for (auto& shape : shapes)
        {
            size_t indexOffset = 0;
            for (auto& faceVertexCount : shape.mesh.num_face_vertices)
            {
                if (faceVertexCount != 3)
                {
                    break;
                }

                auto indices = shape.mesh.indices[indexOffset + 0];

                real x0 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 0];
                real y0 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 1];
                real z0 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 2];

                real normalX0 = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 0];
                real normalY0 = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 1];
                real normalZ0 = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 2];

                indices = shape.mesh.indices[indexOffset + 1];

                real x1 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 0];
                real y1 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 1];
                real z1 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 2];

                real normalX1 = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 0];
                real normalY1 = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 1];
                real normalZ1 = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 2];

                indices = shape.mesh.indices[indexOffset + 2];

                real x2 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 0];
                real y2 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 1];
                real z2 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 2];

                real normalX2 = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 0];
                real normalY2 = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 1];
                real normalZ2 = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 2];

                Vector3 vertex0{x0, y0, z0};
                Vector3 vertex1{x1, y1, z1};
                Vector3 vertex2{x2, y2, z2};

                vertex0 = Matrix4x4::Multiply(vertex0, 1.0f, transformation);
                vertex1 = Matrix4x4::Multiply(vertex1, 1.0f, transformation);
                vertex2 = Matrix4x4::Multiply(vertex2, 1.0f, transformation);

                meshBoundingBox = meshBoundingBox.Union(vertex0).Union(vertex1).Union(vertex2);

                Vector3 normal0{normalX0, normalY0, normalZ0};
                Vector3 normal1{normalX1, normalY1, normalZ1};
                Vector3 normal2{normalX2, normalY2, normalZ2};

                normal0 = Matrix4x4::Multiply(normal0, 0.0f, transformation);
                normal1 = Matrix4x4::Multiply(normal1, 0.0f, transformation);
                normal2 = Matrix4x4::Multiply(normal2, 0.0f, transformation);

                auto triangle = std::make_shared<const Triangle>(vertex0, vertex1, vertex2, normal0, normal1, normal2, material);
                sceneConfig.Geometries.push_back(triangle);

                triangles.push_back(triangle.get());

                indexOffset += faceVertexCount;
            }
        }

        // Create the bounding box hierarchy.
        BoundingBoxBuildParameters parameters{};
        auto hierarchy = BuildUniformBoundingBoxHierarchy(parameters, triangles, sceneConfig.Geometries);

        // Build a bounding box around the hierarchy.
        auto axisAlignedBox = std::make_shared<const AxisAlignedBox>(hierarchy->CalculateBoundingBox(), material);
        sceneConfig.Geometries.push_back(axisAlignedBox);

        auto boundingGeometry = std::make_shared<const BoundingGeometry>(axisAlignedBox.get(), hierarchy);
        sceneConfig.Geometries.push_back(boundingGeometry);

        return boundingGeometry.get();
    }

    static std::vector<std::tuple<std::string, bool, bool, std::function<const IntersectableGeometry* (const Node&, const MaterialMap&, SceneConfig&, std::vector<const IntersectableGeometry*>*)>>> GeometryMapFunctions
    {
        {"sphere", true, false, &ParseSphereNode},
        {"plane", true, false, &ParsePlaneNode},
        {"parallelogram", true, false, &ParseParallelogramNode},
        {"triangle", true, false, &ParseTriangleNode},
        {"disc", true, true, &ParseDiscNode},
        {"axisAlignedBox", true, false, &ParseAxisAlignedBoxNode},
        {"cylinder", true, true, &ParseCylinderNode},
        {"geometryCollection", false, true, &ParseGeometryCollectionNode},
        {"boundingGeometry", false, true, &ParseBoundingGeometryNode},
        {"transformed", true, true, &ParseTransformedGeometryNode},
        {"triangleMeshObj", true, true, &ParseTriangleMeshObjNode},
    };

    std::tuple<const IntersectableGeometry*, bool> ParseGeometryNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig, std::vector<const IntersectableGeometry*>* sequenceGeometries, bool geometryOnly)
    {
        for (const auto& [nodeName, isGeometry, shouldInclude, functionPointer] : GeometryMapFunctions)
        {
            if (geometryOnly && !isGeometry)
            {
                continue;
            }

            auto childNode = node[nodeName];
            if (childNode)
            {
                auto geometry = functionPointer(childNode, materialMap, sceneConfig, sequenceGeometries);
                return std::make_tuple(geometry, shouldInclude);
            }
        }

        return std::make_tuple(nullptr, false);
    }

    std::shared_ptr<std::vector<const IntersectableGeometry*>> ParseGeometrySequenceNode(const Node& node, const MaterialMap& materialMap, SceneConfig& sceneConfig)
    {
        std::vector<const IntersectableGeometry*> sequenceGeometries{};
        auto geometries = std::shared_ptr<std::vector<const IntersectableGeometry*>>{new std::vector<const IntersectableGeometry*>{}};

        for (const Node& childNode : node)
        {
            auto [geometry, shouldInclude] = ParseGeometryNode(childNode, materialMap, sceneConfig, &sequenceGeometries, false);
            if (shouldInclude)
            {
                geometries->push_back(geometry);
            }
        }

        CreateGeometrySoaStructures(sequenceGeometries, *geometries, sceneConfig.Geometries);

        return geometries;
    }

    export std::shared_ptr<SceneConfig> ParseSceneNode(const Node& node, const MaterialMap& materialMap)
    {
        auto sceneConfig = std::shared_ptr<SceneConfig>(new SceneConfig{});

        auto [geometry, _] = ParseGeometryNode(node["geometry"], materialMap, *sceneConfig, nullptr, false);
        sceneConfig->Geometry = geometry;

        return sceneConfig;
    }
}