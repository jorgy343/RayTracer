import RayTracer.LambertianMaterial;
import RayTracer.PerspectiveCamera;
import RayTracer.Scene;
import RayTracer.Sphere;
import RayTracer.Plane;
import RayTracer.AxisAlignedBox;
import RayTracer.Parallelogram;
import RayTracer.PointLight;
import RayTracer.ParallelogramLight;
import RayTracer.Vector2;

#include <memory>
#include <vector>
#include <cmath>

using namespace RayTracer;

extern "C" __declspec(dllexport) void __cdecl TraceScene(UIntVector2 screenSize, UIntVector2 inclusiveStartingPoint, UIntVector2 inclusiveEndingPoint, int subpixelCount, int iterations, float* pixelBuffer)
{
    int subpixelCountSquared = subpixelCount * subpixelCount;

    PerspectiveCamera perspectiveCamera{
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0},
        90.0f};

    std::vector<Ray> rayBuffer{};
    rayBuffer.reserve((inclusiveEndingPoint.X - inclusiveStartingPoint.X + 1) * (inclusiveEndingPoint.Y - inclusiveStartingPoint.Y + 1) * subpixelCountSquared);

    Scene scene{{0.0f, 0.0f, 0.0f}};

    LambertianMaterial whiteMaterial{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
    LambertianMaterial redMaterial{{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
    LambertianMaterial greenMaterial{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
    LambertianMaterial blueMaterial{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
    LambertianMaterial orangeMaterial{{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
    LambertianMaterial pinkMaterial{{1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
    LambertianMaterial yellowMaterial{{0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};

    Sphere sphere1{{-2, 0, 5}, 2, &whiteMaterial};
    Sphere sphere2{{0, 0, 7}, 2, &whiteMaterial};
    Sphere sphere3{{2, 0, 5}, 2, &whiteMaterial};

    scene.AddGeometry(&sphere1);
    scene.AddGeometry(&sphere2);
    scene.AddGeometry(&sphere3);

    Plane plane1{Vector3{0.0f, 0.0f, -1.0f}.Normalize(), {0.0f, 0.0f, 20.0f}, &redMaterial};
    Plane plane2{Vector3{0.0f, 0.0f, 1.0f}.Normalize(), {0.0f, 0.0f, -20.0f}, &greenMaterial};
    Plane plane3{Vector3{0.0f, -1.0f, 0.0f}.Normalize(), {0.0f, 20.0f, 0.0f}, &blueMaterial};
    Plane plane4{Vector3{0.0f, 1.0f, 0.0f}.Normalize(), {0.0f, -20.0f, 0.0f}, &orangeMaterial};
    Plane plane5{Vector3{-1.0f, 0.0f, 0.0f}.Normalize(), {20.0f, 0.0f, 0.0f}, &pinkMaterial};
    Plane plane6{Vector3{1.0f, 0.0f, 0.0f}.Normalize(), {-20.0f, 0.0f, 0.0f}, &yellowMaterial};

    scene.AddGeometry(&plane1);
    scene.AddGeometry(&plane2);
    scene.AddGeometry(&plane3);
    scene.AddGeometry(&plane4);
    scene.AddGeometry(&plane5);
    scene.AddGeometry(&plane6);

    AxisAlignedBox axisAlignedBox{{-8, -2, 5}, {-6, 2, 9}, &whiteMaterial};
    scene.AddGeometry(&axisAlignedBox);

    Parallelogram parallelogram1{{5.0f, 0.0f, 4.0f}, {0.0f, -3.0f, 0.3f}, {2.0f, 0.0f, 0.0f}, &whiteMaterial};
    scene.AddGeometry(&parallelogram1);

    //PointLight light1{{1.0f, 1.0f, 1.0f}, {-1.0f, 10.0f, 0.0f}};
    ParallelogramLight light2{{1.0f, 1.0f, 1.0f}, {{0, 19.9f, -6}, {-3, 0, 0}, {0, 0, 3}, nullptr}};

    //scene.AddLight(&light1);
    scene.AddLight(&light2);

    scene.Finalize();

    for (int count = 0; count < iterations; count++)
    {
        perspectiveCamera.CreateRays(screenSize, inclusiveStartingPoint, inclusiveEndingPoint, subpixelCount, rayBuffer);
        int rayBufferIndex = 0;

        for (unsigned int y = inclusiveStartingPoint.Y; y <= inclusiveEndingPoint.Y; y++)
        {
            for (unsigned int x = inclusiveStartingPoint.X; x <= inclusiveEndingPoint.X; x++)
            {
                Vector3 color{};

                for (int subpixelY = 0; subpixelY < subpixelCount; subpixelY++)
                {
                    for (int subpixelX = 0; subpixelX < subpixelCount; subpixelX++)
                    {
                        Ray& ray = rayBuffer[rayBufferIndex++];
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
            pixelBuffer[((y * screenSize.X) + x) * 4 + 0] /= iterations;
            pixelBuffer[((y * screenSize.X) + x) * 4 + 1] /= iterations;
            pixelBuffer[((y * screenSize.X) + x) * 4 + 2] /= iterations;
            pixelBuffer[((y * screenSize.X) + x) * 4 + 3] /= iterations;
        }
    }
}