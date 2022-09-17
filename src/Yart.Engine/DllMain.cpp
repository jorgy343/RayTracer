import AxisAlignedBox;
import EmissiveMaterial;
import GgxMaterial;
import LambertianMaterial;
import LambertianMaterial2;
import MirrorMaterial;
import Parallelogram;
import PerspectiveCamera;
import Plane;
import PlaneSoa;
import Scene;
import Sphere;
import SphereSoa;
import TransformedGeometry;
import Vector2;
import Vector3;

#include <memory>
#include <vector>
#include <cmath>

#include "Vcl.h"

using namespace Yart;

extern "C" __declspec(dllexport) void __cdecl TraceScene(UIntVector2 screenSize, UIntVector2 inclusiveStartingPoint, UIntVector2 inclusiveEndingPoint, unsigned int subpixelCount, unsigned int iterations, float* pixelBuffer)
{
	int subpixelCountSquared = subpixelCount * subpixelCount;

	PerspectiveCamera perspectiveCamera{
		{0, 0, 0},
		{0, 0, 1},
		{0, 1, 0},
		subpixelCount,
		screenSize,
		90.0f};

	Scene scene{{0.0f, 0.0f, 0.0f}};

	// Materials
	constexpr float intensity = 1.0f;
	constexpr bool enableRoulette = false;

	LambertianMaterial2<enableRoulette> whiteMaterial{{intensity, intensity, intensity}};
	LambertianMaterial2<enableRoulette> redMaterial{{intensity, 0.0f, 0.0f}};
	LambertianMaterial2<enableRoulette> greenMaterial{{0.0f, intensity, 0.0f}};
	LambertianMaterial2<enableRoulette> blueMaterial{{0.0f, 0.0f, intensity}};
	LambertianMaterial2<enableRoulette> orangeMaterial{{intensity, intensity, 0.0f}};
	LambertianMaterial2<enableRoulette> pinkMaterial{{intensity, 0.0f, intensity}};
	LambertianMaterial2<enableRoulette> yellowMaterial{{0.0f, intensity, intensity}};

	MirrorMaterial mirrorMaterial{};
	EmissiveMaterial whiteLightMaterial{Vector3{1.0f}};

	// Lights
	Parallelogram parallelogramLight{{0, 19.9f, -6}, {-1.2f, 0, 0}, {0, 0, 1.2f}, &whiteLightMaterial};

	scene.AddAreaLight(&parallelogramLight);
	scene.AddGeometry(&parallelogramLight);

	// Geometry
	GgxMaterial metalMaterial{Vector3{1.0f}, Vector3{0.4f}, 0.07f};

	Sphere sphere1{{-2, 0, 5}, 2, &whiteMaterial};
	Sphere sphere2{{0, 0, 7}, 2, &whiteMaterial};
	Sphere sphere3{{2, 0, 5}, 2, &whiteMaterial};

	SphereSoa sphereSoa1{};

	//sphereSoa1.Insert(0, &sphere1);
	sphereSoa1.Insert(1, &sphere2);
	sphereSoa1.Insert(2, &sphere3);

	scene.AddGeometry(&sphereSoa1);

	TransformedGeometry transformedGeometry1{&sphere1, Matrix4x4::CreateScale(1.0f, 1.0f, 1.0f)};
	scene.AddGeometry(&transformedGeometry1);

	Plane plane1{Vector3{0.0f, 0.0f, -1.0f}.Normalize(), {0.0f, 0.0f, 20.0f}, &redMaterial};
	Plane plane2{Vector3{0.0f, 0.0f, 1.0f}.Normalize(), {0.0f, 0.0f, -20.0f}, &greenMaterial};
	Plane plane3{Vector3{0.0f, -1.0f, 0.0f}.Normalize(), {0.0f, 20.0f, 0.0f}, &blueMaterial};
	Plane plane4{Vector3{0.0f, 1.0f, 0.0f}.Normalize(), {0.0f, -20.0f, 0.0f}, &orangeMaterial};
	Plane plane5{Vector3{-1.0f, 0.0f, 0.0f}.Normalize(), {20.0f, 0.0f, 0.0f}, &pinkMaterial};
	Plane plane6{Vector3{1.0f, 0.0f, 0.0f}.Normalize(), {-20.0f, 0.0f, 0.0f}, &yellowMaterial};

	PlaneSoa planeSoa1{};

	planeSoa1.Insert(0, &plane1);
	planeSoa1.Insert(1, &plane2);
	planeSoa1.Insert(2, &plane3);
	planeSoa1.Insert(3, &plane4);
	planeSoa1.Insert(4, &plane5);
	planeSoa1.Insert(5, &plane6);

	scene.AddGeometry(&planeSoa1);

	AxisAlignedBox axisAlignedBox{{-8, -2, 5}, {-6, 2, 9}, &whiteMaterial};
	scene.AddGeometry(&axisAlignedBox);

	Parallelogram parallelogram1{{5.0f, 0.0f, 4.0f}, {0.0f, -3.0f, 0.3f}, {2.0f, 0.4f, 0.2f}, &greenMaterial};
	scene.AddGeometry(&parallelogram1);

	for (unsigned int count = 0; count < iterations; count++)
	{
		for (unsigned int y = inclusiveStartingPoint.Y; y <= inclusiveEndingPoint.Y; y++)
		{
			for (unsigned int x = inclusiveStartingPoint.X; x <= inclusiveEndingPoint.X; x++)
			{
				Vector3 color{};

				for (unsigned int subpixelY = 0; subpixelY < subpixelCount; subpixelY++)
				{
					for (unsigned int subpixelX = 0; subpixelX < subpixelCount; subpixelX++)
					{
						Ray ray = perspectiveCamera.CreateRay({x, y}, {subpixelX, subpixelY});
						color += scene.CastRayColor(ray);
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
			pixelBuffer[((y * screenSize.X) + x) * 4 + 0] /= static_cast<float>(iterations);
			pixelBuffer[((y * screenSize.X) + x) * 4 + 1] /= static_cast<float>(iterations);
			pixelBuffer[((y * screenSize.X) + x) * 4 + 2] /= static_cast<float>(iterations);
			pixelBuffer[((y * screenSize.X) + x) * 4 + 3] /= static_cast<float>(iterations);
		}
	}
}

//consteval bool Test()
//{
//    PerspectiveCamera perspectiveCamera{
//        {0, 0, 0},
//        {0, 0, 1},
//        {0, 1, 0},
//        90.0f};
//
//    std::vector<Ray> rayBuffer{};
//    Scene scene{{0.0f, 0.0f, 0.0f}};
//
//    LambertianMaterial whiteMaterial{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
//    LambertianMaterial redMaterial{{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
//    LambertianMaterial greenMaterial{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
//    LambertianMaterial blueMaterial{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
//    LambertianMaterial orangeMaterial{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
//    LambertianMaterial pinkMaterial{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
//    LambertianMaterial yellowMaterial{{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
//
//    Sphere sphere1{{-2, 0, 5}, 2, &whiteMaterial};
//    Sphere sphere2{{0, 0, 7}, 2, &whiteMaterial};
//    Sphere sphere3{{2, 0, 5}, 2, &whiteMaterial};
//
//    scene.AddGeometry(&sphere1);
//    scene.AddGeometry(&sphere2);
//    scene.AddGeometry(&sphere3);
//
//    Plane plane1{Vector3{0.0f, 0.0f, -1.0f}.Normalize(), {0.0f, 0.0f, 20.0f}, &redMaterial};
//    Plane plane2{Vector3{0.0f, 0.0f, 1.0f}.Normalize(), {0.0f, 0.0f, -20.0f}, &greenMaterial};
//    Plane plane3{Vector3{0.0f, -1.0f, 0.0f}.Normalize(), {0.0f, 20.0f, 0.0f}, &blueMaterial};
//    Plane plane4{Vector3{0.0f, 1.0f, 0.0f}.Normalize(), {0.0f, -20.0f, 0.0f}, &orangeMaterial};
//    Plane plane5{Vector3{-1.0f, 0.0f, 0.0f}.Normalize(), {20.0f, 0.0f, 0.0f}, &pinkMaterial};
//    Plane plane6{Vector3{1.0f, 0.0f, 0.0f}.Normalize(), {-20.0f, 0.0f, 0.0f}, &yellowMaterial};
//
//    scene.AddGeometry(&plane1);
//    scene.AddGeometry(&plane2);
//    scene.AddGeometry(&plane3);
//    scene.AddGeometry(&plane4);
//    scene.AddGeometry(&plane5);
//    scene.AddGeometry(&plane6);
//
//    Parallelogram parallelogram1{{5.0f, 0.0f, 4.0f}, {0.0f, -3.0f, 0.3f}, {2.0f, 0.0f, 0.0f}, &whiteMaterial};
//    scene.AddGeometry(&parallelogram1);
//
//    PointLight light1{{1.0f, 1.0f, 1.0f}, {-1.0f, 10.0f, 0.0f}};
//    //ParallelogramLight light2{{1.0f, 1.0f, 1.0f}, {{0, 19.9f, -6}, {-3, 0, 0}, {0, 0, 3}, nullptr}};
//
//    scene.AddLight(&light1);
//    //scene.AddLight(&light2);
//
//    UIntVector2 screenSize{10, 10};
//    UIntVector2 inclusiveStartingPoint{0, 0};
//    UIntVector2 inclusiveEndingPoint{screenSize.X - 1, screenSize.Y - 1};
//    int subpixelCount{1};
//    int iterations{1};
//
//    int subpixelCountSquared = subpixelCount * subpixelCount;
//
//    std::vector<float> pixelBuffer{};
//    pixelBuffer.reserve(screenSize.X * screenSize.Y * 4);
//
//    for (int count = 0; count < iterations; count++)
//    {
//        perspectiveCamera.CreateRays(screenSize, inclusiveStartingPoint, inclusiveEndingPoint, subpixelCount, rayBuffer);
//        int rayBufferIndex = 0;
//
//        for (unsigned int y = inclusiveStartingPoint.Y; y <= inclusiveEndingPoint.Y; y++)
//        {
//            for (unsigned int x = inclusiveStartingPoint.X; x <= inclusiveEndingPoint.X; x++)
//            {
//                Vector3 color{};
//
//                for (int subpixelY = 0; subpixelY < subpixelCount; subpixelY++)
//                {
//                    for (int subpixelX = 0; subpixelX < subpixelCount; subpixelX++)
//                    {
//                        Ray& ray = rayBuffer[rayBufferIndex++];
//                        //color += scene.CastRayColor(ray);
//                    }
//                }
//
//                color /= static_cast<float>(subpixelCountSquared);
//
//                //pixelBuffer[((y * screenSize.X) + x) * 4 + 0] += color.X;
//                //pixelBuffer[((y * screenSize.X) + x) * 4 + 1] += color.Y;
//                //pixelBuffer[((y * screenSize.X) + x) * 4 + 2] += color.Z;
//                //pixelBuffer[((y * screenSize.X) + x) * 4 + 3] += 0.0f;
//            }
//        }
//    }
//
//    for (unsigned int y = inclusiveStartingPoint.Y; y <= inclusiveEndingPoint.Y; y++)
//    {
//        for (unsigned int x = inclusiveStartingPoint.X; x <= inclusiveEndingPoint.X; x++)
//        {
//            //pixelBuffer[((y * screenSize.X) + x) * 4 + 0] /= static_cast<float>(iterations);
//            //pixelBuffer[((y * screenSize.X) + x) * 4 + 1] /= static_cast<float>(iterations);
//            //pixelBuffer[((y * screenSize.X) + x) * 4 + 2] /= static_cast<float>(iterations);
//            //pixelBuffer[((y * screenSize.X) + x) * 4 + 3] /= static_cast<float>(iterations);
//        }
//    }
//
//    return true;
//}
//
////static_assert(Test());