#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "SphereSoa.h"

using namespace RayTracer;

int main()
{
    auto sphere1 = Sphere(Vector3(10, 0, 0), 2);
    auto sphere2 = Sphere(Vector3(12, 0, 0), 2);
    auto sphere3 = Sphere(Vector3(14, 0, 0), 2);
    auto sphere4 = Sphere(Vector3(16, 0, 0), 2);
    auto sphere5 = Sphere(Vector3(18, 0, 0), 2);
    auto sphere6 = Sphere(Vector3(20, 0, 0), 2);
    auto sphere7 = Sphere(Vector3(22, 0, 0), 2);
    auto sphere8 = Sphere(Vector3(24, 0, 0), 2);

    auto sphereSoa = SphereSoa();

    sphereSoa.AddSphere(&sphere2);
    sphereSoa.AddSphere(&sphere3);
    sphereSoa.AddSphere(&sphere4);
    sphereSoa.AddSphere(&sphere5);
    sphereSoa.AddSphere(&sphere6);
    sphereSoa.AddSphere(&sphere1); // This is the sphere that will be hit.
    sphereSoa.AddSphere(&sphere7);
    sphereSoa.AddSphere(&sphere8);

    auto ray = Ray(Vector3(2, 0, 0), Vector3(1, 0, 0));

    auto epochIterations = 10000000;

    ankerl::nanobench::Bench()
        .epochIterations(epochIterations)
        .run("Sphere.Intersect(Ray)", [&]
            {
                auto result = sphere1.Intersect(ray);

                ankerl::nanobench::doNotOptimizeAway(result);
            });

    ankerl::nanobench::Bench()
        .epochIterations(epochIterations)
        .run("Sphere.Intersect(Ray)", [&]
            {
                auto result = sphereSoa.Intersect(ray);

                ankerl::nanobench::doNotOptimizeAway(result);
            });
}