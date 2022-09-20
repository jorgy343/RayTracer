import AxisAlignedBox;
import EmissiveMaterial;
import GeometryCollection;
import GgxMaterial;
import LambertianMaterial;
import LambertianMaterial2;
import Math;
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

import YamlLoader;
import YamlNodes;

#include <memory>
#include <vector>
#include <cmath>

#include "Vcl.h"

using namespace Yart;

extern "C" __declspec(dllexport) void __cdecl TraceScene(UIntVector2 screenSize, UIntVector2 inclusiveStartingPoint, UIntVector2 inclusiveEndingPoint, unsigned int subpixelCount, unsigned int iterations, float* pixelBuffer)
{
	Yaml::Config config = Yaml::LoadYaml();



	int subpixelCountSquared = subpixelCount * subpixelCount;

	PerspectiveCamera perspectiveCamera{
		{0, 0, -39},
		{0, 0, 1},
		{0, 1, 0},
		subpixelCount,
		screenSize,
		Math::deg_to_rad(45.0f)};

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

	EmissiveMaterial whiteLightMaterial{Vector3{1.0f}};
	MirrorMaterial mirrorMaterial{};
	GgxMaterial ggxMaterial{Vector3{1.0f}, Vector3{0.4f}, 0.07f};

	// Lights
	Parallelogram parallelogramLight{{0, 19.9f, -6}, {-1.2f, 0, 0}, {0, 0, 1.2f}, &whiteLightMaterial};

	scene.AddAreaLight(&parallelogramLight);
	scene.AddGeometry(&parallelogramLight);

	// Geometry
	Plane plane1{Vector3{0.0f, 0.0f, -1.0f}.Normalize(), {0.0f, 0.0f, 20.0f}, &redMaterial};
	Plane plane2{Vector3{0.0f, 0.0f, 1.0f}.Normalize(), {0.0f, 0.0f, -40.0f}, &greenMaterial};
	Plane plane3{Vector3{0.0f, -1.0f, 0.0f}.Normalize(), {0.0f, 20.0f, 0.0f}, &blueMaterial};
	Plane plane4{Vector3{0.0f, 1.0f, 0.0f}.Normalize(), {0.0f, -20.0f, 0.0f}, &orangeMaterial};
	Plane plane5{Vector3{-1.0f, 0.0f, 0.0f}.Normalize(), {20.0f, 0.0f, 0.0f}, &pinkMaterial};
	Plane plane6{Vector3{1.0f, 0.0f, 0.0f}.Normalize(), {-20.0f, 0.0f, 0.0f}, &yellowMaterial};

	PlaneSoa planeSoa1
	{
		&plane1,
		&plane2,
		&plane3,
		&plane4,
		&plane5,
		&plane6,
	};

	scene.AddGeometry(&planeSoa1);

	Sphere sphere01{{-16.0f, -16.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere02{{-08.0f, -16.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere03{{+00.0f, -16.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere04{{+08.0f, -16.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere05{{+16.0f, -16.0f, +18.0f}, 3.0f, &whiteMaterial};

	Sphere sphere06{{-16.0f, -08.0f, +18.0f}, 3.0f, &mirrorMaterial};
	Sphere sphere07{{-08.0f, -08.0f, +18.0f}, 3.0f, &mirrorMaterial};
	Sphere sphere08{{+00.0f, -08.0f, +18.0f}, 3.0f, &mirrorMaterial};
	Sphere sphere09{{+08.0f, -08.0f, +18.0f}, 3.0f, &mirrorMaterial};
	Sphere sphere10{{+16.0f, -08.0f, +18.0f}, 3.0f, &mirrorMaterial};

	Sphere sphere11{{-16.0f, +00.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere12{{-08.0f, +00.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere13{{+00.0f, +00.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere14{{+08.0f, +00.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere15{{+16.0f, +00.0f, +18.0f}, 3.0f, &whiteMaterial};

	Sphere sphere16{{-16.0f, +08.0f, +18.0f}, 3.0f, &ggxMaterial};
	Sphere sphere17{{-08.0f, +08.0f, +18.0f}, 3.0f, &ggxMaterial};
	Sphere sphere18{{+00.0f, +08.0f, +18.0f}, 3.0f, &ggxMaterial};
	Sphere sphere19{{+08.0f, +08.0f, +18.0f}, 3.0f, &ggxMaterial};
	Sphere sphere20{{+16.0f, +08.0f, +18.0f}, 3.0f, &ggxMaterial};

	Sphere sphere21{{-16.0f, +16.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere22{{-08.0f, +16.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere23{{+00.0f, +16.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere24{{+08.0f, +16.0f, +18.0f}, 3.0f, &whiteMaterial};
	Sphere sphere25{{+16.0f, +16.0f, +18.0f}, 3.0f, &whiteMaterial};

	SphereSoa sphereSoa1
	{
		&sphere01,
		&sphere02,
		&sphere03,
		&sphere04,
		&sphere05,
		&sphere06,
		&sphere07,
		&sphere08,
	};

	SphereSoa sphereSoa2
	{
		&sphere09,
		&sphere10,
		&sphere11,
		&sphere12,
		&sphere13,
		&sphere14,
		&sphere15,
		&sphere16,
	};

	SphereSoa sphereSoa3
	{
		&sphere17,
		&sphere18,
		&sphere19,
		&sphere20,
		&sphere21,
		&sphere22,
		&sphere23,
		&sphere24,
	};

	scene.AddGeometry(&sphereSoa1);
	scene.AddGeometry(&sphereSoa2);
	scene.AddGeometry(&sphereSoa3);
	scene.AddGeometry(&sphere25);

	// Execute ray tracing.
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
						Vector3 sampledColor = scene.CastRayColor(ray);

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
			pixelBuffer[((y * screenSize.X) + x) * 4 + 0] /= static_cast<float>(iterations);
			pixelBuffer[((y * screenSize.X) + x) * 4 + 1] /= static_cast<float>(iterations);
			pixelBuffer[((y * screenSize.X) + x) * 4 + 2] /= static_cast<float>(iterations);
			pixelBuffer[((y * screenSize.X) + x) * 4 + 3] /= static_cast<float>(iterations);
		}
	}
}