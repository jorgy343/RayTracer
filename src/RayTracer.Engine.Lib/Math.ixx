#include "Vcl.h"

export module RayTracer.Math;

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

    export inline float FastSqrt(float value)
    {
        __m128 mmValue = _mm_load_ss(&value);
        _mm_store_ss(&value, _mm_sqrt_ps(mmValue));

        return value;
    }

    export inline float FastReciprical(float value)
    {
        __m128 mmValue = _mm_load_ss(&value);
        _mm_store_ss(&value, _mm_rcp_ss(mmValue));

        return value;
    }

    export inline float FastMax(float value1, float value2)
    {
        __m128 mmValue1 = _mm_load_ss(&value1);
        __m128 mmValue2 = _mm_load_ss(&value2);

        _mm_store_ss(&value1, _mm_max_ss(mmValue1, mmValue2));

        return value1;
    }

    export inline float FastMin(float value1, float value2)
    {
        __m128 mmValue1 = _mm_load_ss(&value1);
        __m128 mmValue2 = _mm_load_ss(&value2);

        _mm_store_ss(&value1, _mm_min_ss(mmValue1, mmValue2));

        return value1;
    }
}