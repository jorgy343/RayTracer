module;

#include "nanobench.h"

export module Bench.Matrix3x3Bench;

import Bench.Config;
import Matrix3x3;

namespace Yart::Bench
{
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

    export void RunMatrix3x3Bench()
    {
        ankerl::nanobench::Bench{}
            .epochIterations(DefaultEpochIterations)
            .run("Matrix3x3.operator*(Matrix3x3)", [&]
                {
                    Matrix3x3 result = matrix3x3_1 * matrix3x3_2;
                    ankerl::nanobench::doNotOptimizeAway(result);
                });
    }
}