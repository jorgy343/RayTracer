module;

#include "nanobench.h"
#include "Vcl.h"

export module RayTracer.Bench.AxisAlignedBoxBench;

import RayTracer.Bench.Config;
import RayTracer.AxisAlignedBox;
import RayTracer.AxisAlignedBoxSoa;
import RayTracer.IntersectionResultType;

namespace RayTracer::Bench
{
    AxisAlignedBox box1{{2, -4, -4}, {4, 4, 4}, nullptr};
    AxisAlignedBox box2{{6, -4, -4}, {8, 4, 4}, nullptr};
    AxisAlignedBox box3{{10, -4, -4}, {12, 4, 4}, nullptr};
    AxisAlignedBox box4{{14, -4, -4}, {16, 4, 4}, nullptr};
    AxisAlignedBox box5{{18, -4, -4}, {20, 4, 4}, nullptr};
    AxisAlignedBox box6{{22, -4, -4}, {24, 4, 4}, nullptr};
    AxisAlignedBox box7{{26, -4, -4}, {28, 4, 4}, nullptr};
    AxisAlignedBox box8{{30, -4, -4}, {32, 4, 4}, nullptr};

    AxisAlignedBoxSoa axisAlignedBoxSoa{};

    Ray rayMiss{{-2, 0, 0}, {-1, 0, 0}};
    Ray rayHit{{-2, 0, 0}, {1, 0, 0}};

    export void RunAxisAlignedBoxBench()
    {
        int i = 0;

        axisAlignedBoxSoa.Insert(i++, &box2);
        axisAlignedBoxSoa.Insert(i++, &box3);
        axisAlignedBoxSoa.Insert(i++, &box4);
        axisAlignedBoxSoa.Insert(i++, &box5);
        axisAlignedBoxSoa.Insert(i++, &box6);
        axisAlignedBoxSoa.Insert(i++, &box1); // This is the sphere that will be hit.
        axisAlignedBoxSoa.Insert(i++, &box7);
        axisAlignedBoxSoa.Insert(i++, &box8);

        ankerl::nanobench::Bench()
            .epochIterations(DefaultEpochIterations)
            .run("AxisAlignedBox.Intersect(Ray)", [&]
                {
                    auto result1 = box1.IntersectEntrance(rayMiss);
                    ankerl::nanobench::doNotOptimizeAway(result1);

                    auto result2 = box1.IntersectEntrance(rayHit);
                    ankerl::nanobench::doNotOptimizeAway(result2);
                });

        ankerl::nanobench::Bench()
            .epochIterations(DefaultEpochIterations)
            .run("AxisAlignedBoxSoa.Intersect(Ray)", [&]
                {
                    auto result1 = axisAlignedBoxSoa.IntersectEntrance(rayMiss);
                    ankerl::nanobench::doNotOptimizeAway(result1);

                    auto result2 = axisAlignedBoxSoa.IntersectEntrance(rayHit);
                    ankerl::nanobench::doNotOptimizeAway(result2);
                });
    }
}