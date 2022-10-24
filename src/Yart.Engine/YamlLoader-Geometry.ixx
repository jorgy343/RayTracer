module;

#include "range/v3/view/chunk.hpp"

#include "yaml-cpp/yaml.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Vcl.h"

export module YamlLoader:Geometry;

import <functional>;

import "Common.h";

import :Materials;
import :Matrices;
import :Vectors;
import AreaLight;
import AxisAlignedBox;
import AxisAlignedBoxSoa;
import BoundingBoxHierarchy;
import BoundingGeometry;
import Disc;
import GeometryCollection;
import GeometrySoa;
import GeometrySoaUtilities;
import IntersectableGeometry;
import Material;
import Math;
import MixedMaterial;
import Parallelogram;
import ParallelogramSoa;
import Plane;
import PlaneSoa;
import RayMarcher;
import SignedDistance;
import SignedDistanceBinaryOperation;
import SignedDistanceBinaryOperator;
import SignedDistanceCylinder;
import SignedDistanceRoundedAxisAlignedBox;
import Sphere;
import SphereSoa;
import TransformedGeometry;
import Triangle;
import TriangleSoa;

using namespace YAML;

namespace Yart::Yaml
{
    export class ParseGeometryResults
    {
    public:
        std::vector<std::shared_ptr<const IntersectableGeometry>> Geometries{};
        std::vector<const AreaLight*> AreaLights{};

        std::vector<std::shared_ptr<const SignedDistance>> SignedDistances{};
        std::vector<std::shared_ptr<const Material>> AdditionalMaterials{};

        const IntersectableGeometry* Geometry{};
    };

    std::tuple<const IntersectableGeometry*, bool> ParseGeometryNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries, bool geometryOnly);
    std::shared_ptr<std::vector<const IntersectableGeometry*>> ParseGeometrySequenceNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults);

    std::tuple<const SignedDistance*, bool> ParseSignedDistanceNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults);
    std::shared_ptr<std::vector<const SignedDistance*>> ParseSignedDistanceGeometrySequenceNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults);

    const Sphere* ParseSphereNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName);

        auto position = ParseVector3(node["position"]);
        auto radius = node["radius"].as<real>();

        auto geometry = std::make_shared<const Sphere>(position, radius, material);
        parseGeometryResults.Geometries.push_back(geometry);

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const Plane* ParsePlaneNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName);

        auto normal = ParseVector3(node["normal"]);
        auto point = ParseVector3(node["point"]);

        auto geometry = std::make_shared<const Plane>(normal, point, material);
        parseGeometryResults.Geometries.push_back(geometry);

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const Parallelogram* ParseParallelogramNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto areaLight = node["areaLight"].as<bool>(false);

        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName);

        auto position = ParseVector3(node["position"]);
        auto edge1 = ParseVector3(node["edge1"]);
        auto edge2 = ParseVector3(node["edge2"]);

        auto geometry = std::make_shared<const Parallelogram>(position, edge1, edge2, material);
        parseGeometryResults.Geometries.push_back(geometry);

        if (areaLight)
        {
            parseGeometryResults.AreaLights.push_back(geometry.get());
        }

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const Triangle* ParseTriangleNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName);

        auto vertex0 = ParseVector3(node["vertex0"]);
        auto vertex1 = ParseVector3(node["vertex1"]);
        auto vertex2 = ParseVector3(node["vertex2"]);

        std::shared_ptr<const Triangle> geometry;

        auto normalNode0 = node["normal0"];
        auto normalNode1 = node["normal1"];
        auto normalNode2 = node["normal2"];

        if (normalNode0 && normalNode1 && normalNode2)
        {
            auto normal0 = ParseVector3(normalNode0);
            auto normal1 = ParseVector3(normalNode1);
            auto normal2 = ParseVector3(normalNode2);

            geometry = std::make_shared<const Triangle>(vertex0, vertex1, vertex2, normal0, normal1, normal2, material);
        }
        else
        {
            geometry = std::make_shared<const Triangle>(vertex0, vertex1, vertex2, material);
        }

        parseGeometryResults.Geometries.push_back(geometry);

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const Disc* ParseDiscNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto areaLight = node["areaLight"].as<bool>(false);

        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName);

        auto position = ParseVector3(node["position"]);
        auto normal = ParseVector3(node["normal"]);
        auto radius = node["radius"].as<real>();

        auto geometry = std::make_shared<const Disc>(position, normal, radius, material);
        parseGeometryResults.Geometries.push_back(geometry);

        if (areaLight)
        {
            parseGeometryResults.AreaLights.push_back(geometry.get());
        }

        return geometry.get();
    }

    const AxisAlignedBox* ParseAxisAlignedBoxNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName);

        auto minimum = ParseVector3(node["minimum"]);
        auto maximum = ParseVector3(node["maximum"]);

        auto geometry = std::make_shared<const AxisAlignedBox>(minimum, maximum, material);
        parseGeometryResults.Geometries.push_back(geometry);

        if (sequenceGeometries)
        {
            sequenceGeometries->push_back(geometry.get());
        }

        return geometry.get();
    }

    const GeometryCollection* ParseGeometryCollectionNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto children = ParseGeometrySequenceNode(node["children"], materialMap, parseGeometryResults);

        auto geometry = std::make_shared<const GeometryCollection>(*children);
        parseGeometryResults.Geometries.push_back(geometry);

        return geometry.get();
    }

    const BoundingGeometry* ParseBoundingGeometryNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto [child, ignored1] = ParseGeometryNode(node["child"], materialMap, parseGeometryResults, nullptr, false);

        auto boundingVolumeNode = node["boundingVolume"];
        if (boundingVolumeNode)
        {
            auto [boundingVolume, ignored2] = ParseGeometryNode(boundingVolumeNode, materialMap, parseGeometryResults, nullptr, false);

            auto geometry = std::make_shared<const BoundingGeometry>(boundingVolume, child);
            parseGeometryResults.Geometries.push_back(geometry);

            return geometry.get();
        }
        else
        {
            return CreateBoundingGeometryFromGeometry(child, parseGeometryResults.Geometries);
        }
    }

    const TransformedGeometry* ParseTransformedGeometryNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto [childGeometry, ignored] = ParseGeometryNode(node["child"], materialMap, parseGeometryResults, nullptr, true);
        auto matrix = ParseMatrix4x4(node["transformation"]);

        auto geometry = std::make_shared<const TransformedGeometry>(reinterpret_cast<const Geometry*>(childGeometry), matrix);
        parseGeometryResults.Geometries.push_back(geometry);

        return geometry.get();
    }

    const IntersectableGeometry* ParseTriangleMeshObjNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName);

        auto transformation = Matrix4x4::CreateIdentity();

        auto transformationNode = node["transformation"];
        if (transformationNode)
        {
            transformation = ParseMatrix4x4(node["transformation"]);
        }

        // Read the geometry from the obj file.
        auto objFilename = node["objFile"].as<std::string>();

        tinyobj::ObjReader reader{};
        reader.ParseFromFile(objFilename);

        auto& attributes = reader.GetAttrib();
        auto& shapes = reader.GetShapes();

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

                Vector3 normal0{normalX0, normalY0, normalZ0};
                Vector3 normal1{normalX1, normalY1, normalZ1};
                Vector3 normal2{normalX2, normalY2, normalZ2};

                normal0 = Matrix4x4::Multiply(normal0, 0.0f, transformation);
                normal1 = Matrix4x4::Multiply(normal1, 0.0f, transformation);
                normal2 = Matrix4x4::Multiply(normal2, 0.0f, transformation);

                auto triangle = std::make_shared<const Triangle>(vertex0, vertex1, vertex2, normal0, normal1, normal2, material);
                parseGeometryResults.Geometries.push_back(triangle);

                triangles.push_back(triangle.get());

                indexOffset += faceVertexCount;
            }
        }

        // Create the bounding box hierarchy.
        BoundingBoxBuildParameters parameters{};

        return BuildSplitByLongAxisBoundingBoxHierarchy(parameters, triangles, parseGeometryResults.Geometries);
        //return BuildUniformBoundingBoxHierarchy(parameters, triangles, parseGeometryResults.Geometries);
    }

    const RayMarcher* ParseRayMarcherNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto children = ParseSignedDistanceGeometrySequenceNode(node["children"], materialMap, parseGeometryResults);

        auto geometry = std::make_shared<const RayMarcher>(*children);
        parseGeometryResults.Geometries.push_back(geometry);

        return geometry.get();
    }

    const SignedDistanceCylinder* ParseSignedDistanceCylinderNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName);

        auto start = ParseVector3(node["start"]);
        auto end = ParseVector3(node["end"]);
        auto radius = node["radius"].as<real>();

        auto signedDistance = std::make_shared<const SignedDistanceCylinder>(start, end, radius, material);
        parseGeometryResults.SignedDistances.push_back(signedDistance);

        return signedDistance.get();
    }

    const SignedDistanceRoundedAxisAlignedBox* ParseSignedDistanceRoundedAxisAlignedBoxNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto materialName = node["material"].as<std::string>();
        auto material = materialMap.at(materialName);

        auto minimum = ParseVector3(node["minimum"]);
        auto maximum = ParseVector3(node["maximum"]);
        auto radius = node["radius"].as<real>();

        auto signedDistance = std::make_shared<const SignedDistanceRoundedAxisAlignedBox>(minimum, maximum, radius, material);
        parseGeometryResults.SignedDistances.push_back(signedDistance);

        return signedDistance.get();
    }

    const SignedDistance* ParseSignedDistanceBinaryOperationUnionNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto smoothingAmount = node["smoothingAmount"].as<real>(real{0});

        auto left = std::get<0>(ParseSignedDistanceNode(node["left"], materialMap, parseGeometryResults));
        auto right = std::get<0>(ParseSignedDistanceNode(node["right"], materialMap, parseGeometryResults));

        auto mixedMaterial = std::make_shared<const MixedMaterial>(left->GetMaterial(), right->GetMaterial());
        parseGeometryResults.AdditionalMaterials.push_back(mixedMaterial);

        if (smoothingAmount == 0)
        {
            auto signedDistance = std::make_shared<const SignedDistanceBinaryOperation<SignedDistanceBinaryOperator::Union, false>>(smoothingAmount, left, right, mixedMaterial.get());
            parseGeometryResults.SignedDistances.push_back(signedDistance);

            return signedDistance.get();
        }
        else
        {
            auto signedDistance = std::make_shared<const SignedDistanceBinaryOperation<SignedDistanceBinaryOperator::Union, true>>(smoothingAmount, left, right, mixedMaterial.get());
            parseGeometryResults.SignedDistances.push_back(signedDistance);

            return signedDistance.get();
        }
    }

    const SignedDistance* ParseSignedDistanceBinaryOperationIntersectionNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto smoothingAmount = node["smoothingAmount"].as<real>(real{0});

        auto left = std::get<0>(ParseSignedDistanceNode(node["left"], materialMap, parseGeometryResults));
        auto right = std::get<0>(ParseSignedDistanceNode(node["right"], materialMap, parseGeometryResults));

        auto mixedMaterial = std::make_shared<const MixedMaterial>(left->GetMaterial(), right->GetMaterial());
        parseGeometryResults.AdditionalMaterials.push_back(mixedMaterial);

        if (smoothingAmount == 0)
        {
            auto signedDistance = std::make_shared<const SignedDistanceBinaryOperation<SignedDistanceBinaryOperator::Intersection, false>>(smoothingAmount, left, right, mixedMaterial.get());
            parseGeometryResults.SignedDistances.push_back(signedDistance);

            return signedDistance.get();
        }
        else
        {
            auto signedDistance = std::make_shared<const SignedDistanceBinaryOperation<SignedDistanceBinaryOperator::Intersection, true>>(smoothingAmount, left, right, mixedMaterial.get());
            parseGeometryResults.SignedDistances.push_back(signedDistance);

            return signedDistance.get();
        }
    }

    const SignedDistance* ParseSignedDistanceBinaryOperationDifferenceNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries)
    {
        auto smoothingAmount = node["smoothingAmount"].as<real>(real{0});

        auto left = std::get<0>(ParseSignedDistanceNode(node["left"], materialMap, parseGeometryResults));
        auto right = std::get<0>(ParseSignedDistanceNode(node["right"], materialMap, parseGeometryResults));

        auto mixedMaterial = std::make_shared<const MixedMaterial>(left->GetMaterial(), right->GetMaterial());
        parseGeometryResults.AdditionalMaterials.push_back(mixedMaterial);

        if (smoothingAmount == 0)
        {
            auto signedDistance = std::make_shared<const SignedDistanceBinaryOperation<SignedDistanceBinaryOperator::Difference, false>>(smoothingAmount, left, right, mixedMaterial.get());
            parseGeometryResults.SignedDistances.push_back(signedDistance);

            return signedDistance.get();
        }
        else
        {
            auto signedDistance = std::make_shared<const SignedDistanceBinaryOperation<SignedDistanceBinaryOperator::Difference, true>>(smoothingAmount, left, right, mixedMaterial.get());
            parseGeometryResults.SignedDistances.push_back(signedDistance);

            return signedDistance.get();
        }
    }

    static std::vector<std::tuple<std::string, bool, bool, std::function<const IntersectableGeometry* (const Node&, MaterialMap&, ParseGeometryResults&, std::vector<const IntersectableGeometry*>*)>>> GeometryMapFunctions
    {
        {"sphere", true, false, &ParseSphereNode},
        {"plane", true, false, &ParsePlaneNode},
        {"parallelogram", true, false, &ParseParallelogramNode},
        {"triangle", true, false, &ParseTriangleNode},
        {"disc", true, true, &ParseDiscNode},
        {"axisAlignedBox", true, false, &ParseAxisAlignedBoxNode},
        {"geometryCollection", false, true, &ParseGeometryCollectionNode},
        {"boundingGeometry", false, true, &ParseBoundingGeometryNode},
        {"transformed", true, true, &ParseTransformedGeometryNode},
        {"triangleMeshObj", true, true, &ParseTriangleMeshObjNode},
        {"rayMarcher", false, true, ParseRayMarcherNode},
    };

    static std::vector<std::tuple<std::string, std::function<const SignedDistance* (const Node&, MaterialMap&, ParseGeometryResults&, std::vector<const IntersectableGeometry*>*)>>> SignedDistanceMapFunctions
    {
        {"sphere", &ParseSphereNode},
        {"axisAlignedBox", &ParseAxisAlignedBoxNode},
        {"roundedAxisAlignedBox", &ParseSignedDistanceRoundedAxisAlignedBoxNode},
        {"cylinder", &ParseSignedDistanceCylinderNode},
        {"union", &ParseSignedDistanceBinaryOperationUnionNode},
        {"intersection", &ParseSignedDistanceBinaryOperationIntersectionNode},
        {"difference", &ParseSignedDistanceBinaryOperationDifferenceNode},
    };

    std::tuple<const IntersectableGeometry*, bool> ParseGeometryNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults, std::vector<const IntersectableGeometry*>* sequenceGeometries, bool geometryOnly)
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
                auto geometry = functionPointer(childNode, materialMap, parseGeometryResults, sequenceGeometries);
                return std::make_tuple(geometry, shouldInclude);
            }
        }

        return std::make_tuple(nullptr, false);
    }

    std::tuple<const SignedDistance*, bool> ParseSignedDistanceNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults)
    {
        for (const auto& [nodeName, functionPointer] : SignedDistanceMapFunctions)
        {
            auto childNode = node[nodeName];
            if (childNode)
            {
                auto geometry = functionPointer(childNode, materialMap, parseGeometryResults, nullptr);
                return std::make_tuple(geometry, true);
            }
        }

        return std::make_tuple(nullptr, false);
    }

    std::shared_ptr<std::vector<const IntersectableGeometry*>> ParseGeometrySequenceNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults)
    {
        std::vector<const IntersectableGeometry*> sequenceGeometries{};
        auto geometries = std::shared_ptr<std::vector<const IntersectableGeometry*>>{new std::vector<const IntersectableGeometry*>{}};

        for (const Node& childNode : node)
        {
            auto [geometry, shouldInclude] = ParseGeometryNode(childNode, materialMap, parseGeometryResults, &sequenceGeometries, false);
            if (shouldInclude)
            {
                geometries->push_back(geometry);
            }
        }

        CreateGeometrySoaStructures(sequenceGeometries, *geometries, parseGeometryResults.Geometries);

        return geometries;
    }

    std::shared_ptr<std::vector<const SignedDistance*>> ParseSignedDistanceGeometrySequenceNode(const Node& node, MaterialMap& materialMap, ParseGeometryResults& parseGeometryResults)
    {
        auto signedDistances = std::shared_ptr<std::vector<const SignedDistance*>>{new std::vector<const SignedDistance*>{}};

        for (const Node& childNode : node)
        {
            auto [signedDistance, shouldInclude] = ParseSignedDistanceNode(childNode, materialMap, parseGeometryResults);
            if (shouldInclude)
            {
                signedDistances->push_back(signedDistance);
            }
        }

        return signedDistances;
    }

    export std::shared_ptr<ParseGeometryResults> ParseSceneNode(const Node& node, MaterialMap& materialMap)
    {
        auto parseGeometryResults = std::shared_ptr<ParseGeometryResults>(new ParseGeometryResults{});

        auto [geometry, _] = ParseGeometryNode(node, materialMap, *parseGeometryResults, nullptr, false);
        parseGeometryResults->Geometry = geometry;

        return parseGeometryResults;
    }
}