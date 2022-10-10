import AxisAlignedBox;
import BoundingBox;
import BoundingGeometry;
import Camera;
import GeometryCollection;
import IntersectableGeometry;
import LambertianMaterial;
import Light;
import Math;
import Random;
import Scene;
import Triangle;
import TriangleSoa;
import YamlLoader;

#include "range/v3/view/chunk.hpp"

#include "Vcl.h"
#include "Common.h"

using namespace Yart;

class SceneData
{
public:
    std::shared_ptr<Yaml::YamlData> YamlData{};
    std::shared_ptr<Scene> SavedScene{};

    SceneData(
        std::shared_ptr<Yaml::YamlData> yamlData,
        std::shared_ptr<Scene> savedScene)
        :
        YamlData{yamlData},
        SavedScene{savedScene}
    {

    }
};

extern "C" __declspec(dllexport) void* __cdecl CreateScene()
{
    auto yamlData = Yaml::LoadYaml();
    auto scene = std::make_shared<Scene>(yamlData->GeometryData->Geometry, yamlData->MissShader.get());

    for (const auto light : yamlData->Lights)
    {
        scene->AddLight(light.get());
    }

    for (const auto areaLight : yamlData->GeometryData->AreaLights)
    {
        scene->AddAreaLight(areaLight);
    }

    auto sceneData = new SceneData{
        yamlData,
        scene};

    return sceneData;
}

extern "C" __declspec(dllexport) void __cdecl DeleteScene(SceneData * sceneData)
{
    delete sceneData;
}

extern "C" __declspec(dllexport) void __cdecl TraceScene(UIntVector2 screenSize, UIntVector2 inclusiveStartingPoint, UIntVector2 inclusiveEndingPoint, const SceneData * sceneData, float* pixelBuffer)
{
    Random random{};
    Camera& camera = *sceneData->YamlData->Camera;

    int subpixelCountSquared = camera.SubpixelCount * camera.SubpixelCount;
    Vector2 colorClamp = sceneData->YamlData->Config->ColorClamp;

    // Execute ray tracing.
    for (unsigned int count = 0; count < sceneData->YamlData->Config->Iterations; count++)
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

                        sampledColor.X = Math::max(colorClamp.X, Math::min(colorClamp.Y, std::isnan(sampledColor.X) ? real{0.0} : sampledColor.X));
                        sampledColor.Y = Math::max(colorClamp.X, Math::min(colorClamp.Y, std::isnan(sampledColor.Y) ? real{0.0} : sampledColor.Y));
                        sampledColor.Z = Math::max(colorClamp.X, Math::min(colorClamp.Y, std::isnan(sampledColor.Z) ? real{0.0} : sampledColor.Z));

                        color += sampledColor;
                    }
                }

                color /= static_cast<real>(subpixelCountSquared);

                pixelBuffer[((y * screenSize.X) + x) * 4 + 0] += static_cast<float>(color.X);
                pixelBuffer[((y * screenSize.X) + x) * 4 + 1] += static_cast<float>(color.Y);
                pixelBuffer[((y * screenSize.X) + x) * 4 + 2] += static_cast<float>(color.Z);
                pixelBuffer[((y * screenSize.X) + x) * 4 + 3] += 0.0f;
            }
        }
    }

    for (unsigned int y = inclusiveStartingPoint.Y; y <= inclusiveEndingPoint.Y; y++)
    {
        for (unsigned int x = inclusiveStartingPoint.X; x <= inclusiveEndingPoint.X; x++)
        {
            pixelBuffer[((y * screenSize.X) + x) * 4 + 0] /= static_cast<float>(sceneData->YamlData->Config->Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 1] /= static_cast<float>(sceneData->YamlData->Config->Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 2] /= static_cast<float>(sceneData->YamlData->Config->Iterations);
            pixelBuffer[((y * screenSize.X) + x) * 4 + 3] /= static_cast<float>(sceneData->YamlData->Config->Iterations);
        }
    }
}