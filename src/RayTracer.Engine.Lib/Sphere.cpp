#include "Sphere.h"

#include <immintrin.h>

using namespace RayTracer;

Sphere::Sphere()
    : Position(0.0f, 0.0f, 0.0f), _padding(0.0f), Radius(0.0f)
{

}

Sphere::Sphere(const Vector3& position, float radius)
	: Position(position), _padding(0.0f), Radius(radius)
{

}

float RayTracer::Sphere::Intersect(const Ray& ray) const
{
    auto v = ray.Position - Position;

    auto a = ray.Direction.Dot(ray.Direction);
    auto b = v.Dot(ray.Direction);
    auto c = v.Dot(v) - (Radius * Radius);

    auto discriminant = (b * b) - (a * c);

    float discriminantSqrt;
    _mm_store_ss(&discriminantSqrt, _mm_sqrt_ss(_mm_load_ss(&discriminant)));

    float inverseA;
    _mm_store_ss(&inverseA, _mm_rcp_ss(_mm_load_ss(&a)));

    auto negativeB = -b;

    auto exitDistance = (negativeB + discriminantSqrt) * inverseA;

    // For each float if it is greater than or equal to 0.0f, then copy it to final result; otherwise, copy nan to the final result.
    auto nan = _mm_set1_ps(std::numeric_limits<float>::quiet_NaN());
    auto exitDistanceComparison = _mm_cmp_ps(_mm_load_ss(&exitDistance), _mm_setzero_ps(), _CMP_GE_OQ); // exitDistanceComparison = exitDistance >= 0.0f
    auto exitDistanceComparisonResult = _mm_blendv_ps(nan, _mm_load_ss(&exitDistance), exitDistanceComparison);
    _mm_store_ss(&exitDistance, exitDistanceComparisonResult);

    auto entranceDistance = (negativeB - discriminantSqrt) * inverseA;
    auto maxedEntranceDistance = _mm_max_ps(_mm_load_ss(&entranceDistance), _mm_setzero_ps());
    
    float finalResult;
    _mm_store_ss(&finalResult, maxedEntranceDistance);

    return finalResult;
}