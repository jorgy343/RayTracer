import AxisAlignedBox;
import AxisAlignedBoxSoa;
import Camera;
import IntersectableGeometry;
import Math;
import Random;
import Scene;
import Vector2;
import Vector3;
import YamlLoader;

#include <cmath>
#include <memory>
#include <vector>

#include "range/v3/view/chunk.hpp"

#include "Vcl.h"

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

    auto scene = std::make_shared<Scene>(sceneConfig->Geometry, Vector3{0.0f, 0.0f, 0.0f});

    for (auto areaLight : sceneConfig->AreaLights)
    {
        scene->AddAreaLight(areaLight);
    }

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
            pixelBuffer[((y * screenSize.X) + x) * 4 + 0] /= static_cast<float>(sceneData->SavedConfig->Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 1] /= static_cast<float>(sceneData->SavedConfig->Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 2] /= static_cast<float>(sceneData->SavedConfig->Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 3] /= static_cast<float>(sceneData->SavedConfig->Iterations);
        }
    }
}