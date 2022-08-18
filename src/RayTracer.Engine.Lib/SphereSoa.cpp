#include "SphereSoa.h"

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

	__m256 a = DOT_256(rayDirectionX, rayDirectionX, rayDirectionY, rayDirectionY, rayDirectionZ, rayDirectionZ);
	__m256 b = DOT_256(vX, rayDirectionX, vY, rayDirectionY, vZ, rayDirectionZ);

	__m256 sphereRadius = _mm256_load_ps(_radius);
	__m256 sphereRadiusSquared = _mm256_mul_ps(sphereRadius, sphereRadius);

	__m256 vDotV = DOT_256(vX, vX, vY, vY, vZ, vZ);
	__m256 c = _mm256_sub_ps(vDotV, sphereRadiusSquared);

	__m256 discriminant = _mm256_sub_ps(_mm256_mul_ps(b, b), _mm256_mul_ps(a, c));
	__m256 discriminantSqrt = _mm256_sqrt_ps(discriminant);

	__m256 inverseA = _mm256_rcp_ps(a);
	__m256 negativeB = NEGATE_256(b);

	__m256 exitDistance = _mm256_mul_ps(_mm256_add_ps(negativeB, discriminantSqrt), inverseA);

	__m256 nan = _mm256_set1_ps(std::numeric_limits<float>::quiet_NaN());
	__m256 entranceDistance = _mm256_mul_ps(_mm256_sub_ps(negativeB, discriminantSqrt), inverseA);
	__m256 maxEntranceDistance = _mm256_max_ps(entranceDistance, _mm256_setzero_ps());

	// For each float, if it is greater than or equal to 0.0f, then copy it to final result; otherwise, copy nan to the final result.
	__m256 exitDistanceComparison = _mm256_cmp_ps(exitDistance, _mm256_setzero_ps(), _CMP_GE_OQ); // exitDistanceComparison = exitDistance >= 0.0f
	__m256 finalResult = _mm256_blendv_ps(nan, maxEntranceDistance, exitDistanceComparison);

	float minimumDistance;
	HORIZONTAL_MIN_256(finalResult, minimumDistance);

	// Find the index of the minimum distance.
	//
	// Assume the minimum distance found was 4 and the finalResult register holds the following values [7 9 4 5 | 5 9 6 8].
	// The compare instruction below will set all bits in each float that matches the minimum distance found and clear the
	// bits if it doesn't match. So the resulting minimumCompare register would look like [0 0 1 0 | 0 0 0 0].
	__m256 minimumCompare = _mm256_cmp_ps(_mm256_broadcast_ss(&minimumDistance), finalResult, _CMP_EQ_OQ);

	// Creating a mask from the minimumCompare register will give us a byte where each bit is set if the most significant
	// bit in the float is set. With the example above, our mask byte would look like 0b0010'0000.
	int minimumMoveMask = _mm256_movemask_ps(minimumCompare);

	// Set the 8th bit so that we will always return the 8th sphere in the array if no sphere is hit.
	minimumMoveMask |= 0x80;

	// Count the number of least signficant zero bits until a one bit is found. In the example above the index would be 5.
	// Since the 8th bit is set above, we will never get a result larger than 7.
	int minimumIndex = _tzcnt_u32(minimumMoveMask);

	return SphereSoaIntersectionResult(_spheres[minimumIndex], minimumDistance);
}