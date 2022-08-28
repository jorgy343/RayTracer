import RayTracer.PerspectiveCamera;
import RayTracer.Scene;
import RayTracer.Sphere;
import RayTracer.SphereSoa;

#include <memory>
#include <vector>
#include <cmath>

using namespace RayTracer;

extern "C" __declspec(dllexport) void __cdecl TraceScene(int startingX, int startingY, int width, int height, float* pixelBuffer)
{
    PerspectiveCamera perspectiveCamera{
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        90.0f,
        800,
        600};

    std::vector<Ray> rayBuffer(width * height);
    perspectiveCamera.CreateRays(0, 0, width, height, rayBuffer.data());

    Scene scene{{0.0f, 0.0f, 0.3f}};

    Sphere sphere1{{10, 0, 0}, 2};
    Sphere sphere2{{13, 0, 3}, 2};
    Sphere sphere3{{17, 0, -4}, 2};

    scene.AddSphere(&sphere1);
    scene.AddSphere(&sphere2);
    scene.AddSphere(&sphere3);

    scene.Finalize();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Ray ray = rayBuffer[(y * width) + x];

            //IntersectionResult<Sphere> intersectionResult = sphereSoa.Intersect(ray);

            //pixelBuffer[((y * width) + x) * 4 + 0] = std::isinf(intersectionResult.Distance) ? 0.0f : 1.0f;
            pixelBuffer[((y * width) + x) * 4 + 1] = 0.0f;
            pixelBuffer[((y * width) + x) * 4 + 2] = 0.0f;
            pixelBuffer[((y * width) + x) * 4 + 3] = 0.0f;
        }
    }
}