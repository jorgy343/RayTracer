#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"
#include "Vcl.h"

//import Bench.SphereBench;
//import Bench.PlaneBench;
import Bench.AxisAlignedBoxBench;
import Bench.Matrix3x3Bench;
import Bench.Matrix4x4Bench;

using namespace Yart::Bench;

int main()
{
    //RunSphereBench();
    //RunPlaneBench();
    RunAxisAlignedBoxBench();
    RunMatrix3x3Bench();
    RunMatrix4x4Bench();
}