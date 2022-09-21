import AxisAlignedBox;
import Camera;
import EmissiveMaterial;
import GeometryCollection;
import GgxMaterial;
import LambertianMaterial;
import LambertianMaterial2;
import Math;
import MirrorMaterial;
import Parallelogram;
import ParallelogramSoa;
import Plane;
import PlaneSoa;
import Random;
import Scene;
import Sphere;
import SphereSoa;
import TransformedGeometry;
import Vector2;
import Vector3;
import YamlLoader;
import YamlNodes;

#include <cmath>
#include <memory>
#include <vector>

#include "range/v3/view/chunk.hpp"

#include "Vcl.h"

using namespace Yart;

class SceneData
{
public:
    Yaml::Config SavedConfig{};
    std::unique_ptr<Scene> SavedScene{};
    std::unique_ptr<Camera> SavedCamera{};

    SceneData(Yaml::Config savedConfig, std::unique_ptr<Scene> savedScene, std::unique_ptr<Camera> savedCamera)
        : SavedConfig{savedConfig}, SavedScene{std::move(savedScene)}, SavedCamera{std::move(savedCamera)}
    {

    }
};

extern "C" __declspec(dllexport) void* __cdecl CreateScene()
{
    auto [config, camera, sceneConfig] = Yaml::LoadYaml();
    int subpixelCountSquared = config.SubpixelCount * config.SubpixelCount;

    Scene scene{{0.0f, 0.0f, 0.0f}};

    for (const auto& areaLight : sceneConfig->AreaLights)
    {
        scene.AddAreaLight(areaLight);
    }

    auto chunkedSpheres = sceneConfig->Spheres | ranges::views::chunk(8);
    for (const auto& chunkedSphere : chunkedSpheres)
    {
        if (chunkedSphere.size() == 1)
        {
            scene.AddGeometry(chunkedSphere[0]);
        }
        else
        {
            auto soa = std::shared_ptr<SphereSoa>{new SphereSoa{}};

            int index = 0;
            for (const auto& sphere : chunkedSphere)
            {
                soa->Insert(index++, sphere);
            }

            scene.AddGeometry(soa);
        }
    }

    auto chunkedPlanes = sceneConfig->Planes | ranges::views::chunk(8);
    for (const auto& chunkedPlane : chunkedPlanes)
    {
        if (chunkedPlane.size() == 1)
        {
            scene.AddGeometry(chunkedPlane[0]);
        }
        else
        {
            auto soa = std::shared_ptr<PlaneSoa>{new PlaneSoa{}};

            int index = 0;
            for (const auto& plane : chunkedPlane)
            {
                soa->Insert(index++, plane);
            }

            scene.AddGeometry(soa);
        }
    }

    auto chunkedParallelograms = sceneConfig->Parallelograms | ranges::views::chunk(8);
    for (const auto& chunkedParallelogram : chunkedParallelograms)
    {
        if (chunkedParallelogram.size() == 1)
        {
            scene.AddGeometry(chunkedParallelogram[0]);
        }
        else
        {
            auto soa = std::shared_ptr<ParallelogramSoa>{new ParallelogramSoa{}};

            int index = 0;
            for (const auto& parallelogram : chunkedParallelogram)
            {
                soa->Insert(index++, parallelogram);
            }

            scene.AddGeometry(soa);
        }
    }

    auto sceneData = new SceneData{config, std::make_unique<Scene>(scene), std::move(camera)};
    return sceneData;
}

extern "C" __declspec(dllexport) void __cdecl DeleteScene(SceneData* sceneData)
{
    delete sceneData;
}

extern "C" __declspec(dllexport) void __cdecl TraceScene(UIntVector2 screenSize, UIntVector2 inclusiveStartingPoint, UIntVector2 inclusiveEndingPoint, const SceneData* sceneData, float* pixelBuffer)
{
    int subpixelCountSquared = sceneData->SavedConfig.SubpixelCount * sceneData->SavedConfig.SubpixelCount;

    Random random{};

    // Execute ray tracing.
    for (unsigned int count = 0; count < sceneData->SavedConfig.Iterations; count++)
    {
        for (unsigned int y = inclusiveStartingPoint.Y; y <= inclusiveEndingPoint.Y; y++)
        {
            for (unsigned int x = inclusiveStartingPoint.X; x <= inclusiveEndingPoint.X; x++)
            {
                Vector3 color{};

                for (unsigned int subpixelY = 0; subpixelY < sceneData->SavedConfig.SubpixelCount; subpixelY++)
                {
                    for (unsigned int subpixelX = 0; subpixelX < sceneData->SavedConfig.SubpixelCount; subpixelX++)
                    {
                        Ray ray = sceneData->SavedCamera->CreateRay({x, y}, {subpixelX, subpixelY});
                        Vector3 sampledColor = sceneData->SavedScene->CastRayColor(ray, random);

                        sampledColor.X = std::isnan(sampledColor.X) ? 0.0f : sampledColor.X;
                        sampledColor.Y = std::isnan(sampledColor.Y) ? 0.0f : sampledColor.Y;
                        sampledColor.Z = std::isnan(sampledColor.Z) ? 0.0f : sampledColor.Z;

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
            pixelBuffer[((y * screenSize.X) + x) * 4 + 0] /= static_cast<float>(sceneData->SavedConfig.Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 1] /= static_cast<float>(sceneData->SavedConfig.Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 2] /= static_cast<float>(sceneData->SavedConfig.Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 3] /= static_cast<float>(sceneData->SavedConfig.Iterations);
        }
    }
}