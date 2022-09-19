module;

#include "nanobench.h"

export module Bench.Matrix4x4Bench;

import Bench.Config;
import Matrix4x4;

namespace Yart::Bench
{
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

    export void RunMatrix4x4Bench()
    {
        ankerl::nanobench::Bench{}
            .epochIterations(DefaultEpochIterations)
            .run("Matrix4x4.operator*(Matrix4x4)", [&]
                {
                    Matrix4x4 result = matrix4x4_1 * matrix4x4_2;
                    ankerl::nanobench::doNotOptimizeAway(result);
                });
    }
}