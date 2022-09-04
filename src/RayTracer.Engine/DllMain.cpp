import RayTracer.LambertianMaterial;
import RayTracer.PerspectiveCamera;
import RayTracer.Scene;
import RayTracer.Sphere;
import RayTracer.Plane;

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

    Scene scene{{0.0f, 0.0f, 0.0f}};

    LambertianMaterial material{{1.0f, 0.0f, 0.0f}, {0.1f, 0.1f, 0.5f}};

    Sphere sphere1{{5, 0, -2}, 2, &material};
    Sphere sphere2{{7, 0, 0}, 2, &material};
    Sphere sphere3{{5, 0, 2}, 2, &material};

    scene.AddSphere(&sphere1);
    scene.AddSphere(&sphere2);
    scene.AddSphere(&sphere3);

    Plane plane1{{-1.0f, 0.0f, 0.0f}, {12.0f, 0.0f, 0.0f}, &material};

    scene.AddPlane(&plane1);

    DirectionalLight light1{{1.0f, 1.0f, 1.0f}, Vector3{0.3f, -1.0f, 0.0f}.Normalize()};

    scene.AddDirectionalLight(&light1);

    scene.Finalize();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Ray ray = rayBuffer[(y * width) + x];

            Vector3 color = scene.CastRayColor(ray);

            pixelBuffer[((y * width) + x) * 4 + 0] = color.X;
            pixelBuffer[((y * width) + x) * 4 + 1] = color.Y;
            pixelBuffer[((y * width) + x) * 4 + 2] = color.Z;
            pixelBuffer[((y * width) + x) * 4 + 3] = 0.0f;
        }
    }
}