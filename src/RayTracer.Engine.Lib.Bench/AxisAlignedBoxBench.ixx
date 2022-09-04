#include "nanobench.h"

export module RayTracer.Bench.AxisAlignedBoxBench;

import RayTracer.Bench.Config;
import RayTracer.AxisAlignedBox;

namespace RayTracer::Bench
{
    AxisAlignedBox box1{{-5, -5, -5}, {5, 5, 5}, nullptr};

    Ray rayMiss{{-8, 0, 0}, {-1, 0, 0}};
    Ray rayHit{{-8, 0, 0}, {1, 0, 0}};

    export void RunAxisAlignedBoxBench()
    {
        ankerl::nanobench::Bench()
            .epochIterations(DefaultEpochIterations)
            .run("AxisAlignedBox.Intersect(Ray)", [&]
                {
                    auto result1 = box1.Intersect(rayMiss);
                    ankerl::nanobench::doNotOptimizeAway(result1);

                    auto result2 = box1.Intersect(rayHit);
                    ankerl::nanobench::doNotOptimizeAway(result2);
                });
    }
}