#include "nanobench.h"
#include "Vcl.h"

export module RayTracer.Bench.PlaneBench;

import RayTracer.Bench.Config;
import RayTracer.Plane;
import RayTracer.PlaneSoa;
import RayTracer.IntersectionResultType;

namespace RayTracer::Bench
{
    Plane plane1{{-1, 0, 0}, 5, nullptr};
    Plane plane2{{-1, 0, 0}, 6, nullptr};
    Plane plane3{{-1, 0, 0}, 7, nullptr};
    Plane plane4{{-1, 0, 0}, 8, nullptr};
    Plane plane5{{-1, 0, 0}, 9, nullptr};
    Plane plane6{{-1, 0, 0}, 10, nullptr};
    Plane plane7{{-1, 0, 0}, 11, nullptr};
    Plane plane8{{-1, 0, 0}, 12, nullptr};

    PlaneSoa planeSoa{};

    Ray rayMiss{{2, 0, 0}, {1, 0, 0}};
    Ray rayHit{{2, 0, 0}, {-1, 0, 0}};

    export void RunPlaneBench()
    {
        planeSoa.Add(&plane2);
        planeSoa.Add(&plane3);
        planeSoa.Add(&plane4);
        planeSoa.Add(&plane5);
        planeSoa.Add(&plane6);
        planeSoa.Add(&plane1); // This is the sphere that will be hit.
        planeSoa.Add(&plane7);
        planeSoa.Add(&plane8);

        planeSoa.Finalize();

        ankerl::nanobench::Bench()
            .epochIterations(DefaultEpochIterations)
            .run("Plane.Intersect(Ray)", [&]
                {
                    auto result1 = plane1.IntersectEntrance(rayMiss);
                    ankerl::nanobench::doNotOptimizeAway(result1);

                    auto result2 = plane1.IntersectEntrance(rayHit);
                    ankerl::nanobench::doNotOptimizeAway(result2);
                });

        ankerl::nanobench::Bench()
            .epochIterations(DefaultEpochIterations)
            .run("PlaneSoa.Intersect(Ray)", [&]
                {
                    auto result1 = planeSoa.PrivateIntersectSoa<IntersectionResultType::Entrance>(rayMiss, 0);
                    ankerl::nanobench::doNotOptimizeAway(result1);

                    auto result2 = planeSoa.PrivateIntersectSoa<IntersectionResultType::Entrance>(rayHit, 0);
                    ankerl::nanobench::doNotOptimizeAway(result2);
                });
    }
}