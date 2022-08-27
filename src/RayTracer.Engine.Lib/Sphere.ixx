#include <immintrin.h>
#include <limits>

export module RayTracer.Sphere;

import RayTracer.Vector3;
import RayTracer.Ray;

namespace RayTracer
{
    export class alignas(16) Sphere
    {
    public:
        Vector3 Position{};

    private:
        [[maybe_unused]] float _padding{0.0f};

    public:
        float Radius{0.0f};

        Sphere() = default;

        Sphere(const Vector3& position, float radius)
            : Position{position}, _padding{0.0f}, Radius{radius}
        {

        }

        float Intersect(const Ray& ray) const
        {
            Vector3 v = ray.Position - Position;

            float a = ray.Direction * ray.Direction;
            float b = v * ray.Direction;
            float c = (v * v) - (Radius * Radius);

            float discriminant = (b * b) - (a * c);

            float discriminantSqrt;
            _mm_store_ss(&discriminantSqrt, _mm_sqrt_ss(_mm_load_ss(&discriminant)));

            float inverseA;
            _mm_store_ss(&inverseA, _mm_rcp_ss(_mm_load_ss(&a)));

            float negativeB = -b;

            float exitDistance = (negativeB + discriminantSqrt) * inverseA;

            // For each float if it is greater than or equal to 0.0f, then copy it to final result; otherwise, copy nan to the final result.
            __m128 nan = _mm_set1_ps(std::numeric_limits<float>::quiet_NaN());
            __m128 exitDistanceComparison = _mm_cmp_ps(_mm_load_ss(&exitDistance), _mm_setzero_ps(), _CMP_GE_OQ); // exitDistanceComparison = exitDistance >= 0.0f
            __m128 exitDistanceComparisonResult = _mm_blendv_ps(nan, _mm_load_ss(&exitDistance), exitDistanceComparison);
            _mm_store_ss(&exitDistance, exitDistanceComparisonResult);

            float entranceDistance = (negativeB - discriminantSqrt) * inverseA;
            __m128 maxedEntranceDistance = _mm_max_ps(_mm_load_ss(&entranceDistance), _mm_setzero_ps());

            float finalResult;
            _mm_store_ss(&finalResult, maxedEntranceDistance);

            return finalResult;
        }
    };
}