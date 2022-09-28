import AxisAlignedBox;
import BoundingBox;
import BoundingGeometry;
import Camera;
import GeometryCollection;
import IntersectableGeometry;
import LambertianMaterial;
import Math;
import Matrix4x4;
import Random;
import Scene;
import Triangle;
import TriangleSoa;
import Vector2;
import Vector3;
import YamlLoader;

#include <cmath>
#include <limits>
#include <memory>
#include <vector>

#include "range/v3/view/chunk.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Vcl.h"
#include "Common.h"

using namespace Yart;

class SceneData
{
public:
    std::shared_ptr<Yaml::Config> SavedConfig{};
    std::shared_ptr<Yaml::SceneConfig> SavedSceneConfig{};
    std::shared_ptr<Yaml::MaterialMap> SavedMaterialMap{};
    std::shared_ptr<Scene> SavedScene{};
    std::shared_ptr<Camera> SavedCamera{};

    SceneData(
        std::shared_ptr<Yaml::Config> savedConfig,
        std::shared_ptr<Yaml::SceneConfig> savedSceneConfig,
        std::shared_ptr<Yaml::MaterialMap> savedMaterialMap,
        std::shared_ptr<Scene> savedScene,
        std::shared_ptr<Camera> savedCamera)
        :
        SavedConfig{savedConfig},
        SavedSceneConfig{savedSceneConfig},
        SavedMaterialMap{savedMaterialMap},
        SavedScene{savedScene},
        SavedCamera{savedCamera}
    {

    }
};

extern "C" __declspec(dllexport) void* __cdecl CreateScene()
{
    auto [config, camera, sceneConfig, materialMap] = Yaml::LoadYaml();

    auto scene = std::make_shared<Scene>(sceneConfig->Geometry, config->BackgroundColor);

    for (auto areaLight : sceneConfig->AreaLights)
    {
        scene->AddAreaLight(areaLight);
    }















    const LambertianMaterial<false>* material = new LambertianMaterial<false>{Vector3{1.0f}};
    std::vector<const IntersectableGeometry*> triangles{};

    tinyobj::ObjReaderConfig config2{};
    config2.mtl_search_path = "../../../../Yart.Engine/";

    tinyobj::ObjReader reader{};

    reader.ParseFromFile("../../../../Yart.Engine/teapot.obj", config2);

    auto& attributes = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    BoundingBox box{
        Vector3{std::numeric_limits<float>::infinity()},
        Vector3{-std::numeric_limits<float>::infinity()},
    };

    Matrix4x4 transform = Matrix4x4::CreateScale(2.0f) * Matrix4x4::CreateRotationY(Math::deg_to_rad(90.0f)) * Matrix4x4::CreateTranslation(0.0f, -7.0f, -10.0f);

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

            indices = shape.mesh.indices[indexOffset + 1];

            real x1 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 0];
            real y1 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 1];
            real z1 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 2];

            indices = shape.mesh.indices[indexOffset + 2];

            real x2 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 0];
            real y2 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 1];
            real z2 = attributes.vertices[3 * static_cast<size_t>(indices.vertex_index) + 2];

            Vector3 vertex0{x0, y0, z0};
            Vector3 vertex1{x1, y1, z1};
            Vector3 vertex2{x2, y2, z2};

            vertex0 = Matrix4x4::Multiply(vertex0, 1.0f, transform);
            vertex1 = Matrix4x4::Multiply(vertex1, 1.0f, transform);
            vertex2 = Matrix4x4::Multiply(vertex2, 1.0f, transform);

            box = box.Union(vertex0).Union(vertex1).Union(vertex2);

            if (indices.normal_index >= 0)
            {
                real normalX = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 0];
                real normalY = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 1];
                real normalZ = attributes.normals[3 * static_cast<size_t>(indices.normal_index) + 2];

                Vector3 normal{normalX, normalY, normalZ};
                normal = Matrix4x4::Multiply(normal, 0.0f, transform);
                normal.Normalize();

                triangles.push_back(new Triangle{
                    vertex0,
                    vertex1,
                    vertex2,
                    material});
            }
            else
            {
                triangles.push_back(new Triangle{
                    vertex0,
                    vertex1,
                    vertex2,
                    material});
            }

            indexOffset += faceVertexCount;
        }
    }

    const AxisAlignedBox* boundingBox = new AxisAlignedBox{box, material};
    const GeometryCollection* geometryCollection = new GeometryCollection{triangles};
    const BoundingGeometry* boundingGeometry = new BoundingGeometry{boundingBox, geometryCollection};

    GeometryCollection* col = const_cast<GeometryCollection*>(reinterpret_cast<const GeometryCollection*>(scene->RootGeometry));
    col->ChildGeometries.push_back(boundingGeometry);














    auto sceneData = new SceneData{
        config,
        sceneConfig,
        materialMap,
        scene,
        camera};

    return sceneData;
}

extern "C" __declspec(dllexport) void __cdecl DeleteScene(SceneData * sceneData)
{
    delete sceneData;
}

extern "C" __declspec(dllexport) void __cdecl TraceScene(UIntVector2 screenSize, UIntVector2 inclusiveStartingPoint, UIntVector2 inclusiveEndingPoint, const SceneData * sceneData, float* pixelBuffer)
{
    Random random{};
    Camera& camera = *sceneData->SavedCamera;

    int subpixelCountSquared = camera.SubpixelCount * camera.SubpixelCount;
    Vector2 colorClamp = sceneData->SavedConfig->ColorClamp;

    // Execute ray tracing.
    for (unsigned int count = 0; count < sceneData->SavedConfig->Iterations; count++)
    {
        for (unsigned int y = inclusiveStartingPoint.Y; y <= inclusiveEndingPoint.Y; y++)
        {
            for (unsigned int x = inclusiveStartingPoint.X; x <= inclusiveEndingPoint.X; x++)
            {
                Vector3 color{};

                for (unsigned int subpixelY = 0; subpixelY < camera.SubpixelCount; subpixelY++)
                {
                    for (unsigned int subpixelX = 0; subpixelX < camera.SubpixelCount; subpixelX++)
                    {
                        Ray ray = camera.CreateRay({x, y}, {subpixelX, subpixelY}, random);
                        Vector3 sampledColor = sceneData->SavedScene->CastRayColor(ray, random);

                        sampledColor.X = Math::max(colorClamp.X, Math::min(colorClamp.Y, std::isnan(sampledColor.X) ? 0.0f : sampledColor.X));
                        sampledColor.Y = Math::max(colorClamp.X, Math::min(colorClamp.Y, std::isnan(sampledColor.Y) ? 0.0f : sampledColor.Y));
                        sampledColor.Z = Math::max(colorClamp.X, Math::min(colorClamp.Y, std::isnan(sampledColor.Z) ? 0.0f : sampledColor.Z));

                        color += sampledColor;
                    }
                }

                color /= static_cast<float>(subpixelCountSquared);

                pixelBuffer[((y * screenSize.X) + x) * 4 + 0] += color.X;
                pixelBuffer[((y * screenSize.X) + x) * 4 + 1] += color.Y;
                pixelBuffer[((y * screenSize.X) + x) * 4 + 2] += color.Z;
                pixelBuffer[((y * screenSize.X) + x) * 4 + 3] += 0.0f;
            }
        }
    }

    for (unsigned int y = inclusiveStartingPoint.Y; y <= inclusiveEndingPoint.Y; y++)
    {
        for (unsigned int x = inclusiveStartingPoint.X; x <= inclusiveEndingPoint.X; x++)
        {
            pixelBuffer[((y * screenSize.X) + x) * 4 + 0] /= static_cast<float>(sceneData->SavedConfig->Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 1] /= static_cast<float>(sceneData->SavedConfig->Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 2] /= static_cast<float>(sceneData->SavedConfig->Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 3] /= static_cast<float>(sceneData->SavedConfig->Iterations);
        }
    }
}