#pragma once

#define VCL_NAMESPACE vcl
#define MAX_VECTOR_SIZE 256

#include "vectorclass.h"
#include "vectormath_exp.h"
#include "ranvec1.h"

namespace vcl
{
    static inline Vec2d approx_recipr(Vec2d const a)
    {
        return Vec2d{1.0} / a;
    }

    static inline Vec4d approx_recipr(Vec4d const a)
    {
        return Vec4d{1.0} / a;
    }
}