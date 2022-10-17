#pragma once

#include <concepts>

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

    static inline Vec2d approx_rsqrt(Vec2d const a)
    {
        return Vec2d{1.0} / sqrt(a);
    }

    static inline Vec4d approx_rsqrt(Vec4d const a)
    {
        return Vec4d{1.0} / sqrt(a);
    }
}

template <typename T>
concept vec_type = std::same_as<vcl::Vec4f, T> || std::same_as<vcl::Vec8f, T> || std::same_as<vcl::Vec2d, T> || std::same_as<vcl::Vec4d, T>;

#ifdef USE_DOUBLE
using real_vec = vcl::Vec4d;
#else
using real_vec = vcl::Vec8f;
#endif