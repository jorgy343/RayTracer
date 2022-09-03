#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

import RayTracer.Bench.SphereBench;
import RayTracer.Bench.PlaneBench;
import RayTracer.Bench.Matrix3x3Bench;
import RayTracer.Bench.Matrix4x4Bench;

using namespace RayTracer::Bench;

int main()
{
    RunSphereBench();
    RunPlaneBench();
    RunMatrix3x3Bench();
    RunMatrix4x4Bench();
}