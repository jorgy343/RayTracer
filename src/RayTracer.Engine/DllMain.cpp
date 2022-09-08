import RayTracer.LambertianMaterial;
import RayTracer.PerspectiveCamera;
import RayTracer.Scene;
import RayTracer.Sphere;
import RayTracer.Plane;
import RayTracer.AxisAlignedBox;
import RayTracer.PointLight;

#include <memory>
#include <vector>
#include <cmath>

using namespace RayTracer;

extern "C" __declspec(dllexport) void __cdecl TraceScene(int startingX, int startingY, int width, int height, float* pixelBuffer)
{
    constexpr int totalIterations = 4;
    constexpr int subpixelCount = 8;
    constexpr int subpixelCountSquared = subpixelCount * subpixelCount;

    PerspectiveCamera perspectiveCamera{
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0},
        90.0f,
        800,
        600};

    std::vector<Ray> rayBuffer(width * height * subpixelCount * subpixelCount);

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

    //DirectionalLight light1{{1.0f, 1.0f, 1.0f}, Vector3{0.0f, -1.0f, 0.3f}.Normalize()};
    PointLight light1{{1.0f, 1.0f, 1.0f}, {0.0f, 10.0f, 0.0f}};

    scene.AddLight(&light1);

    scene.Finalize();

    for (int count = 0; count < totalIterations; count++)
    {
        perspectiveCamera.CreateRays(0, 0, width, height, subpixelCount, rayBuffer.data());

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                //if (x != 310 || y != 350)
                //{
                //    continue;
                //}

                Vector3 color{};

                for (int subpixelX = 0; subpixelX < subpixelCount; subpixelX++)
                {
                    for (int subpixelY = 0; subpixelY < subpixelCount; subpixelY++)
                    {
                        Ray ray = rayBuffer[((y * width) + x) * subpixelCountSquared + (subpixelY * subpixelCount) + subpixelX];
                        color += scene.CastRayColor(ray);
                    }
                }

                color /= static_cast<float>(subpixelCountSquared);

                pixelBuffer[((y * width) + x) * 4 + 0] += color.X;
                pixelBuffer[((y * width) + x) * 4 + 1] += color.Y;
                pixelBuffer[((y * width) + x) * 4 + 2] += color.Z;
                pixelBuffer[((y * width) + x) * 4 + 3] += 0.0f;
            }
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            pixelBuffer[((y * width) + x) * 4 + 0] /= totalIterations;
            pixelBuffer[((y * width) + x) * 4 + 1] /= totalIterations;
            pixelBuffer[((y * width) + x) * 4 + 2] /= totalIterations;
            pixelBuffer[((y * width) + x) * 4 + 3] /= totalIterations;
        }
    }
}