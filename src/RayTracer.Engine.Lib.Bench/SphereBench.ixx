module;

#include "nanobench.h"
#include "Vcl.h"

export module RayTracer.Bench.SphereBench;

import RayTracer.Bench.Config;
import RayTracer.Sphere;
import RayTracer.SphereSoa;
import RayTracer.IntersectionResultType;

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
        int i = 0;

        sphereSoa.Insert(i++, &sphere2);
        sphereSoa.Insert(i++, &sphere3);
        sphereSoa.Insert(i++, &sphere4);
        sphereSoa.Insert(i++, &sphere5);
        sphereSoa.Insert(i++, &sphere6);
        sphereSoa.Insert(i++, &sphere1); // This is the sphere that will be hit.
        sphereSoa.Insert(i++, &sphere7);
        sphereSoa.Insert(i++, &sphere8);

        ankerl::nanobench::Bench()
            .epochIterations(DefaultEpochIterations)
            .run("Sphere.Intersect(Ray)", [&]
                {
                    auto result1 = sphere1.IntersectEntrance(rayMiss);
                    ankerl::nanobench::doNotOptimizeAway(result1);

                    auto result2 = sphere1.IntersectEntrance(rayHit);
                    ankerl::nanobench::doNotOptimizeAway(result2);
                });

        ankerl::nanobench::Bench()
            .epochIterations(DefaultEpochIterations)
            .run("SphereSoa.Intersect(Ray)", [&]
                {
                    auto result1 = sphereSoa.IntersectEntrance(rayMiss);
                    ankerl::nanobench::doNotOptimizeAway(result1);

                    auto result2 = sphereSoa.IntersectEntrance(rayHit);
                    ankerl::nanobench::doNotOptimizeAway(result2);
                });
    }
}