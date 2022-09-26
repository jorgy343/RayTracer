module;

#include "Vcl.h"
#include "gcem.hpp"

export module Math;

import <cmath>;
import <concepts>;

import "Common.h";

import Vec8f3;

using namespace vcl;

namespace Yart
{
    namespace Math
    {
        export template <typename T>
            concept number = std::integral<T> || std::floating_point<T>;

        export template <number T>
            inline constexpr bool isnan(T value)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::internal::is_nan(value);
            }
            else
            {
                return std::isnan(value);
            }
        }

        export template <number T>
            inline constexpr bool isinf(T value)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::internal::is_inf(value);
            }
            else
            {
                return std::isinf(value);
            }
        }

        export template <number T>
            inline constexpr bool isfinite(T value)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::internal::is_finite(value);
            }
            else
            {
                return std::isfinite(value);
            }
        }

        export template <number T>
            inline constexpr T abs(T value)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::abs(value);
            }
            else
            {
                return std::abs(value);
            }
        }

        export template <number T>
            inline constexpr T min(T left, T right)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::min(left, right);
            }
            else if constexpr (std::same_as<float, T>)
            {
                __m128 mmLeft1 = _mm_load_ss(&left);
                __m128 mmRight2 = _mm_load_ss(&right);

                float result;
                _mm_store_ss(&result, _mm_min_ss(mmLeft1, mmRight2));

                return result;
            }
            else if constexpr (std::same_as<double, T>)
            {
                __m128 mmLeft1 = _mm_load_sd(&left);
                __m128 mmRight2 = _mm_load_sd(&right);

                float result;
                _mm_store_sd(&result, _mm_min_sd(mmLeft1, mmRight2));

                return result;
            }
            else
            {
                return std::min(left, right);
            }
        }

        export template <number T>
            inline constexpr T max(T left, T right)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::max(left, right);
            }
            else if constexpr (std::same_as<float, T>)
            {
                __m128 mmLeft1 = _mm_load_ss(&left);
                __m128 mmRight2 = _mm_load_ss(&right);

                float result;
                _mm_store_ss(&result, _mm_max_ss(mmLeft1, mmRight2));

                return result;
            }
            else if constexpr (std::same_as<double, T>)
            {
                __m128 mmLeft1 = _mm_load_sd(&left);
                __m128 mmRight2 = _mm_load_sd(&right);

                float result;
                _mm_store_sd(&result, _mm_max_sd(mmLeft1, mmRight2));

                return result;
            }
            else
            {
                return std::max(left, right);
            }
        }

        export template <std::floating_point T>
            inline constexpr T sqrt(T value)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::sqrt(value);
            }
            else if constexpr (std::same_as<float, T>)
            {
                __m128 mmValue = _mm_load_ss(&value);
                _mm_store_ss(&value, _mm_sqrt_ss(mmValue));

                return value;
            }
            else
            {
                return std::sqrt(value);
            }
        }

        export template <std::floating_point T>
            inline constexpr T rcp(T value)
        {
            if (std::is_constant_evaluated())
            {
                return value == T{0} ? std::numeric_limits<T>::infinity() : T{1} / value;
            }
            else if constexpr (std::same_as<float, T>)
            {
                __m128 mmValue = _mm_load_ss(&value);
                _mm_store_ss(&value, _mm_rcp_ss(mmValue));
                return value;
            }
            else
            {
                return value == T{0} ? std::numeric_limits<T>::infinity() : T{1} / value;
            }
        }

        export template <std::floating_point T>
            inline constexpr T inv_sqrt(T value)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::inv_sqrt(value);
            }
            else if constexpr (std::same_as<float, T>)
            {
                __m128 mmValue = _mm_load_ss(&value);
                _mm_store_ss(&value, _mm_rsqrt_ss(mmValue));

                return value;
            }
            else
            {
                return T{1} / std::sqrt(value);
            }
        }

        export template <std::integral T>
            inline constexpr T pow(T base, T exponent)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::pow(base, exponent);
            }
            else
            {
                return std::pow(static_cast<double>(base), static_cast<double>(exponent));
            }
        }

        export template <std::floating_point T>
            inline constexpr T cos(T radians)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::cos(radians);
            }
            else
            {
                return std::cos(radians);
            }
        }

        export template <std::floating_point T>
            inline constexpr T sin(T radians)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::sin(radians);
            }
            else
            {
                return std::sin(radians);
            }
        }

        export template <std::floating_point T>
            inline constexpr T tan(T radians)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::tan(radians);
            }
            else
            {
                return std::tan(radians);
            }
        }

        export template<number T>
            inline constexpr T deg_to_rad(T degrees)
        {
            return degrees * (Pi / T{180});
        }

        export template<number T>
            inline constexpr T rad_to_deg(T radians)
        {
            return radians * (T{180} / Pi);
        }
    }

    export inline Vec8f SimdDot(
        Vec8f const aX,
        Vec8f const aY,
        Vec8f const aZ,
        Vec8f const bX,
        Vec8f const bY,
        Vec8f const bZ)
    {
        return _mm256_fmadd_ps(aX, bX, _mm256_fmadd_ps(aY, bY, _mm256_mul_ps(aZ, bZ)));
    }

    export inline Vec8f3 SimdCrossProduct(
        Vec8f const aX,
        Vec8f const aY,
        Vec8f const aZ,
        Vec8f const bX,
        Vec8f const bY,
        Vec8f const bZ)
    {
        return
        {
            aY * bZ - aZ * bY,
            aZ * bX - aX * bZ,
            aX * bY - aY * bX,
        };
    }

    export inline Vec4f ConvertNanToInf(Vec4f const value)
    {
        return min(max(value, -infinite4f()), infinite4f());
    }

    export inline Vec8f ConvertNanToInf(Vec8f const value)
    {
        return min(max(value, -infinite8f()), infinite8f());
    }
}