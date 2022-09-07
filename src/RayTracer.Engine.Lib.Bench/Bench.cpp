#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"
#include "Vcl.h"

import RayTracer.Bench.SphereBench;
import RayTracer.Bench.PlaneBench;
import RayTracer.Bench.AxisAlignedBoxBench;
import RayTracer.Bench.Matrix3x3Bench;
import RayTracer.Bench.Matrix4x4Bench;

using namespace RayTracer::Bench;

int main()
{
    RunSphereBench();
    RunPlaneBench();
    RunAxisAlignedBoxBench();
    RunMatrix3x3Bench();
    RunMatrix4x4Bench();
}