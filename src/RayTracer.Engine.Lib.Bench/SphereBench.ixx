#include "nanobench.h"

export module RayTracer.Bench.SphereBench;

import RayTracer.Bench.Config;
import RayTracer.Sphere;
import RayTracer.SphereSoa;

namespace RayTracer::Bench
{
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Sphere sphere2{{12, 0, 0}, 2, nullptr};
    Sphere sphere3{{14, 0, 0}, 2, nullptr};
    Sphere sphere4{{16, 0, 0}, 2, nullptr};
    Sphere sphere5{{18, 0, 0}, 2, nullptr};
    Sphere sphere6{{20, 0, 0}, 2, nullptr};
    Sphere sphere7{{22, 0, 0}, 2, nullptr};
    Sphere sphere8{{24, 0, 0}, 2, nullptr};

    SphereSoa sphereSoa{};

    Ray rayMiss{{2, 0, 0}, {-1, 0, 0}};
    Ray rayHit{{2, 0, 0}, {1, 0, 0}};

    export void RunSphereBench()
    {
        sphereSoa.AddSphere(&sphere2);
        sphereSoa.AddSphere(&sphere3);
        sphereSoa.AddSphere(&sphere4);
        sphereSoa.AddSphere(&sphere5);
        sphereSoa.AddSphere(&sphere6);
        sphereSoa.AddSphere(&sphere1); // This is the sphere that will be hit.
        sphereSoa.AddSphere(&sphere7);
        sphereSoa.AddSphere(&sphere8);

        sphereSoa.Finalize();

        ankerl::nanobench::Bench()
            .epochIterations(DefaultEpochIterations)
            .run("Sphere.Intersect(Ray)", [&]
                {
                    auto result1 = sphere1.Intersect(rayMiss);
                    ankerl::nanobench::doNotOptimizeAway(result1);

                    auto result2 = sphere1.Intersect(rayHit);
                    ankerl::nanobench::doNotOptimizeAway(result2);
                });

        ankerl::nanobench::Bench()
            .epochIterations(DefaultEpochIterations)
            .run("SphereSoa.Intersect(Ray)", [&]
                {
                    auto result1 = sphereSoa.PrivateIntersectSoa(rayMiss, 0);
                    ankerl::nanobench::doNotOptimizeAway(result1);

                    auto result2 = sphereSoa.PrivateIntersectSoa(rayHit, 0);
                    ankerl::nanobench::doNotOptimizeAway(result2);
                });
    }
}