module;

#include "Vcl.h"
#include "gcem.hpp"

export module Math:Basics;

import "Common.h";

using namespace vcl;

namespace Yart
{
    namespace Math
    {
        export template <any_number T>
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

        export template <any_number T>
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

        export template <any_number T>
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

        export template <any_number T>
            inline constexpr T floor(T value)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::floor(value);
            }
            else
            {
                return std::floor(value);
            }
        }

        export template <any_number T>
            inline constexpr T ceil(T value)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::ceil(value);
            }
            else
            {
                return std::ceil(value);
            }
        }

        export template <any_number T>
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

        export template <any_number T>
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
                __m128d mmLeft1 = _mm_load_sd(&left);
                __m128d mmRight2 = _mm_load_sd(&right);

                double result;
                _mm_store_sd(&result, _mm_min_sd(mmLeft1, mmRight2));

                return result;
            }
            else
            {
                return std::min(left, right);
            }
        }

        export template <any_number T>
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
                __m128d mmLeft1 = _mm_load_sd(&left);
                __m128d mmRight2 = _mm_load_sd(&right);

                double result;
                _mm_store_sd(&result, _mm_max_sd(mmLeft1, mmRight2));

                return result;
            }
            else
            {
                return std::max(left, right);
            }
        }

        export template <any_number T>
            inline constexpr T clamp(T value, T min, T max)
        {
            return min(max(value, min), max);
        }

        export template <any_number T>
            inline constexpr T sign(T value)
        {
            return value < T{0} ? T{-1} : T{1};
        }

        export template <any_number T>
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

        export template <real_number T>
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

        export template <real_number T>
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

        export template <any_number T>
            inline constexpr T pow(T base, T exponent)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::pow(base, exponent);
            }
            else
            {
                return std::pow(base, exponent);
            }
        }

        export template <any_number T>
            inline constexpr T exp(T exponent)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::exp(exponent);
            }
            else
            {
                return std::exp(exponent);
            }
        }

        export template <any_number T>
            inline constexpr T log(T exponent)
        {
            if (std::is_constant_evaluated())
            {
                return gcem::log(exponent);
            }
            else
            {
                return std::log(exponent);
            }
        }

        export template <real_number T>
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

        export template <real_number T>
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

        export template <real_number T>
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

        export template<real_number T>
            inline constexpr T deg_to_rad(T degrees)
        {
            return degrees * (Pi / T{180});
        }

        export template<real_number T>
            inline constexpr T rad_to_deg(T radians)
        {
            return radians * (T{180} / Pi);
        }
    }

    export inline Vec4f ConvertNanToInf(Vec4f const value)
    {
        return min(max(value, -infinite4f()), infinite4f());
    }

    export inline Vec8f ConvertNanToInf(Vec8f const value)
    {
        return min(max(value, -infinite8f()), infinite8f());
    }

    export inline Vec2d ConvertNanToInf(Vec2d const value)
    {
        return min(max(value, -infinite2d()), infinite2d());
    }

    export inline Vec4d ConvertNanToInf(Vec4d const value)
    {
        return min(max(value, -infinite4d()), infinite4d());
    }
}