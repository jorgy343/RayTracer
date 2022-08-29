#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

import RayTracing.Matrix3x3;
import RayTracing.Matrix4x4;
import RayTracer.Sphere;
import RayTracer.SphereSoa;

using namespace RayTracer;

int main()
{
    Sphere sphere1{{10, 0, 0}, 2};
    Sphere sphere2{{12, 0, 0}, 2};
    Sphere sphere3{{14, 0, 0}, 2};
    Sphere sphere4{{16, 0, 0}, 2};
    Sphere sphere5{{18, 0, 0}, 2};
    Sphere sphere6{{20, 0, 0}, 2};
    Sphere sphere7{{22, 0, 0}, 2};
    Sphere sphere8{{24, 0, 0}, 2};

    SphereSoa sphereSoa{};

    sphereSoa.AddSphere(&sphere2);
    sphereSoa.AddSphere(&sphere3);
    sphereSoa.AddSphere(&sphere4);
    sphereSoa.AddSphere(&sphere5);
    sphereSoa.AddSphere(&sphere6);
    sphereSoa.AddSphere(&sphere1); // This is the sphere that will be hit.
    sphereSoa.AddSphere(&sphere7);
    sphereSoa.AddSphere(&sphere8);

    sphereSoa.Finalize();

    Ray rayMiss{{2, 0, 0}, {-1, 0, 0}};
    Ray rayHit{{2, 0, 0}, {1, 0, 0}};

    Matrix4x4 matrix4x4_1{
        1,
        2,
        3,
        4,

        5,
        6,
        7,
        8,

        9,
        1,
        2,
        3,

        4,
        5,
        6,
        7,
    };

    Matrix4x4 matrix4x4_2{
        4,
        5,
        6,
        7,

        1,
        2,
        3,
        4,

        5,
        6,
        7,
        8,

        9,
        1,
        2,
        3,
    };

    Matrix3x3 matrix3x3_1{
        1,
        2,
        3,

        5,
        6,
        7,

        9,
        1,
        2,
    };

    Matrix3x3 matrix3x3_2{
        4,
        5,
        6,

        1,
        2,
        3,

        5,
        6,
        7,
    };

    int epochIterations = 10000000;

    ankerl::nanobench::Bench()
        .epochIterations(epochIterations)
        .run("Matrix4x4.operator*(Matrix4x4)", [&]
            {
                Matrix4x4 result = matrix4x4_1 * matrix4x4_2;
                ankerl::nanobench::doNotOptimizeAway(result);
            });

    ankerl::nanobench::Bench()
        .epochIterations(epochIterations)
        .run("Matrix3x3.operator*(Matrix3x3)", [&]
            {
                Matrix3x3 result = matrix3x3_1 * matrix3x3_2;
                ankerl::nanobench::doNotOptimizeAway(result);
            });

    ankerl::nanobench::Bench()
        .epochIterations(epochIterations)
        .run("Sphere.Intersect(Ray)", [&]
            {
                auto result1 = sphere1.Intersect(rayMiss);
                ankerl::nanobench::doNotOptimizeAway(result1);

                auto result2 = sphere1.Intersect(rayHit);
                ankerl::nanobench::doNotOptimizeAway(result2);
            });

    ankerl::nanobench::Bench()
        .epochIterations(epochIterations)
        .run("SphereSoa.Intersect(Ray)", [&]
            {
                auto result1 = sphereSoa.PrivateIntersectSoa(rayMiss, 0);
                ankerl::nanobench::doNotOptimizeAway(result1);

                auto result2 = sphereSoa.PrivateIntersectSoa(rayHit, 0);
                ankerl::nanobench::doNotOptimizeAway(result2);
            });
}