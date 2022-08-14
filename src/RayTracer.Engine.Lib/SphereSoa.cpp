#include "SphereSoa.h"

#include <immintrin.h>
#include "Simd.h"

using namespace RayTracer;

RayTracer::SphereSoaIntersectionResult::SphereSoaIntersectionResult(const RayTracer::Sphere* sphere, float distance)
	: Sphere(sphere), Distance(distance)
{

}

SphereSoa::SphereSoa()
{
	for (auto i = 0; i < 8; i++)
	{
		_positionX[i] = std::numeric_limits<float>::quiet_NaN();
		_positionY[i] = std::numeric_limits<float>::quiet_NaN();
		_positionZ[i] = std::numeric_limits<float>::quiet_NaN();
		_radius[i] = std::numeric_limits<float>::quiet_NaN();
		_spheres[i] = nullptr;
	}

	_count = 0;
}

void SphereSoa::Reset()
{
	_count = 0;
}

void SphereSoa::Finalize()
{
	for (auto i = _count; i < 8; i++)
	{
		_positionX[i] = std::numeric_limits<float>::quiet_NaN();
		_positionY[i] = std::numeric_limits<float>::quiet_NaN();
		_positionZ[i] = std::numeric_limits<float>::quiet_NaN();
		_radius[i] = std::numeric_limits<float>::quiet_NaN();
		_spheres[i] = nullptr;
	}
}

char SphereSoa::GetCount() const
{
	return _count;
}

void SphereSoa::AddSphere(const Sphere* sphere)
{
	if (_count >= 8)
	{
		return;
	}

	_positionX[_count] = sphere->Position.X;
	_positionY[_count] = sphere->Position.Y;
	_positionZ[_count] = sphere->Position.Z;
	_radius[_count] = sphere->Radius;
	_spheres[_count] = sphere;

	_count++;
}

SphereSoaIntersectionResult RayTracer::SphereSoa::Intersect(const Ray& ray) const
{
	auto result = SphereSoaIntersectionResult(nullptr, std::numeric_limits<float>::infinity());

	for (auto i = 0; i < _count; i++)
	{
		auto sphere = _spheres[i];
		auto sphereResult = sphere->Intersect(ray);

		if (sphereResult < result.Distance)
		{
			result.Distance = sphereResult;
			result.Sphere = sphere;
		}
	}

	return result;
}

SphereSoaIntersectionResult RayTracer::SphereSoa::IntersectSimd(const Ray& ray) const
{
	__m256 rayPositionX = _mm256_broadcast_ss(&ray.Position.X);
	__m256 rayPositionY = _mm256_broadcast_ss(&ray.Position.Y);
	__m256 rayPositionZ = _mm256_broadcast_ss(&ray.Position.Z);

	__m256 spherePositionX = _mm256_load_ps(_positionX);
	__m256 spherePositionY = _mm256_load_ps(_positionY);
	__m256 spherePositionZ = _mm256_load_ps(_positionZ);

	__m256 vX = _mm256_sub_ps(rayPositionX, spherePositionX);
	__m256 vY = _mm256_sub_ps(rayPositionY, spherePositionY);
	__m256 vZ = _mm256_sub_ps(rayPositionZ, spherePositionZ);

	__m256 rayDirectionX = _mm256_broadcast_ss(&ray.Direction.X);
	__m256 rayDirectionY = _mm256_broadcast_ss(&ray.Direction.Y);
	__m256 rayDirectionZ = _mm256_broadcast_ss(&ray.Direction.Z);

	__m256 a = DOT256(rayDirectionX, rayDirectionX, rayDirectionY, rayDirectionY, rayDirectionZ, rayDirectionZ);
	__m256 b = DOT256(vX, rayDirectionX, vY, rayDirectionY, vZ, rayDirectionZ);

	__m256 sphereRadius = _mm256_load_ps(_radius);
	__m256 sphereRadiusSquared = _mm256_mul_ps(sphereRadius, sphereRadius);

	__m256 vDotV = DOT256(vX, vX, vY, vY, vZ, vZ);
	__m256 c = _mm256_sub_ps(vDotV, sphereRadiusSquared);

	__m256 discriminant = _mm256_sub_ps(_mm256_mul_ps(b, b), _mm256_mul_ps(a, c));
	__m256 discriminantSqrt = _mm256_sqrt_ps(discriminant);

	__m256 inverseA = _mm256_rcp_ps(a);
	__m256 negativeB = NEGATE256(b);

	__m256 exitDistance = _mm256_mul_ps(_mm256_add_ps(negativeB, discriminantSqrt), inverseA);

	__m256 nan = _mm256_set1_ps(std::numeric_limits<float>::quiet_NaN());
	__m256 entranceDistance = _mm256_mul_ps(_mm256_sub_ps(negativeB, discriminantSqrt), inverseA);
	__m256 maxEntranceDistance = _mm256_max_ps(entranceDistance, _mm256_setzero_ps());

	// For each float if it is greater than or equal to 0.0f, then copy it to final result; otherwise, copy nan to the final result.
	__m256 exitDistanceComparison = _mm256_cmp_ps(exitDistance, _mm256_setzero_ps(), _CMP_GE_OQ); // exitDistanceComparison = exitDistance >= 0.0f
	__m256 finalResult = _mm256_blendv_ps(nan, maxEntranceDistance, exitDistanceComparison);

	// Find the minimum value.
	__m256 v1 = finalResult; // example v1=[1 2 3 4 5 6 7 8]
	__m256 v2 = _mm256_permute_ps(v1, (int)147); // 147 is control code for rotate left by upper 4 elements and lower 4 elements separately v2=[2 3 4 1 6 7 8 5]
	__m256 v3 = _mm256_min_ps(v1, v2); // v3=[2 3 4 4 6 7 8 8]
	__m256 v4 = _mm256_permute_ps(v3, (int)147); // v4=[3 4 4 2 7 8 8 6]
	__m256 v5 = _mm256_min_ps(v3, v4); // v5=[3 4 4 4 7 8 8 8]
	__m256 v6 = _mm256_permute_ps(v5, (int)147); // v6=[4 4 4 3 8 8 8 7]
	__m256 v7 = _mm256_min_ps(v5, v6); // contains max of upper four elements and lower 4 elements. v7=[4 4 4 4 8 8 8 8]
	__m128 v8 = _mm256_extractf128_ps(v7, 1);
	__m128 v9 = _mm_min_ps(_mm256_castps256_ps128(v7), v8);

	alignas(16) float result[4];
	_mm_store_ps(result, v9);

	float minimumDistance = result[3];

	// Find the index of the minimum value.
	__m256 minimumCompare = _mm256_cmp_ps(_mm256_broadcast_ss(&minimumDistance), finalResult, _CMP_EQ_OQ);
	int minimumMoveMask = _mm256_movemask_ps(minimumCompare);
	int minimumIndex = _tzcnt_u32(minimumMoveMask);

	return SphereSoaIntersectionResult(_spheres[minimumIndex], minimumDistance);
}