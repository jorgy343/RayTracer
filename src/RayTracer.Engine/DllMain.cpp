import RayTracer.LambertianMaterial;
import RayTracer.PerspectiveCamera;
import RayTracer.Scene;
import RayTracer.Sphere;
import RayTracer.Plane;
import RayTracer.AxisAlignedBox;

#include <memory>
#include <vector>
#include <cmath>

using namespace RayTracer;

extern "C" __declspec(dllexport) void __cdecl TraceScene(int startingX, int startingY, int width, int height, float* pixelBuffer)
{
    PerspectiveCamera perspectiveCamera{
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0},
        90.0f,
        800,
        600};

    std::vector<Ray> rayBuffer(width * height);

    Scene scene{{0.0f, 0.0f, 0.0f}};

    LambertianMaterial material{{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};

    Sphere sphere1{{-2, 0, 5}, 2, &material};
    Sphere sphere2{{0, 0, 7}, 2, &material};
    Sphere sphere3{{2, 0, 5}, 2, &material};

    scene.AddSphere(&sphere1);
    scene.AddSphere(&sphere2);
    scene.AddSphere(&sphere3);

    Plane plane1{Vector3{0.0f, 0.1f, -1.0f}.Normalize(), {0.0f, 0.0f, 12.0f}, &material};
    scene.AddPlane(&plane1);

    AxisAlignedBox axisAlignedBox{{-8, -2, 5}, {-6, 2, 9}, &material};
    scene.AddAxisAlignedBox(&axisAlignedBox);

    DirectionalLight light1{{1.0f, 1.0f, 1.0f}, Vector3{0.0f, -1.0f, 0.3f}.Normalize()};

    scene.AddDirectionalLight(&light1);

    scene.Finalize();

    constexpr int totalIterations = 20;

    for (int count = 0; count < totalIterations; count++)
    {
        perspectiveCamera.CreateRays(0, 0, width, height, rayBuffer.data());

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                Ray ray = rayBuffer[(y * width) + x];

                Vector3 color = scene.CastRayColor(ray);

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