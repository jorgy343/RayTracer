#include "Vcl.h"

export module RayTracer.Simd;

using namespace vcl;

namespace RayTracer
{
    export inline Vec8f SimdDot(
        Vec8f const aX,
        Vec8f const bX,
        Vec8f const aY,
        Vec8f const bY,
        Vec8f const aZ,
        Vec8f const bZ)
    {
        return _mm256_fmadd_ps(aX, bX, _mm256_fmadd_ps(aY, bY, _mm256_mul_ps(aZ, bZ)));
    }
}