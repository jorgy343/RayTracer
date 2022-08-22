#include "DllMain.h"

using namespace RayTracer;

extern "C" __declspec(dllexport) void __cdecl TraceScene(int startingX, int startingY, int width, int height, float* pixelBuffer)
{
    auto perspectiveCamera = PerspectiveCamera(
        Vector3(0, 0, 0),
        Vector3(1, 0, 0),
        Vector3(0, 1, 0),
        90.0f,
        800,
        600);

    auto rayBuffer = std::make_unique<Ray[]>(width * height);
    perspectiveCamera.CreateRays(0, 0, 800, 600, rayBuffer.get());

    auto sphere1 = Sphere(Vector3(10, 0, 0), 2);
    auto sphere2 = Sphere(Vector3(13, 0, 3), 2);
    auto sphere3 = Sphere(Vector3(17, 0, -4), 2);

    auto sphereSoa = SphereSoa();

    sphereSoa.AddSphere(&sphere1);
    sphereSoa.AddSphere(&sphere2);
    sphereSoa.AddSphere(&sphere3);
    sphereSoa.Finalize();

    for (auto y = 0; y < height; y++)
    {
        for (auto x = 0; x < width; x++)
        {
            auto ray = rayBuffer[(y * width) + x];

            auto intersectionResult = sphereSoa.Intersect(ray);

            pixelBuffer[((y * width) + x) * 4 + 0] = std::isinf(intersectionResult.Distance) ? 0.0f : 1.0f;
            pixelBuffer[((y * width) + x) * 4 + 1] = 0.0f;
            pixelBuffer[((y * width) + x) * 4 + 2] = 0.0f;
            pixelBuffer[((y * width) + x) * 4 + 3] = 0.0f;
        }
    }
}