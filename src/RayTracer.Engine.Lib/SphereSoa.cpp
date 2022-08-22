#include "SphereSoa.h"

#include "Simd.h"

using namespace RayTracer;
using namespace vcl;

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
	Vec8f rayPositionX(ray.Position.X);
	Vec8f rayPositionY(ray.Position.Y);
	Vec8f rayPositionZ(ray.Position.Z);

	Vec8f spherePositionX;
	Vec8f spherePositionY;
	Vec8f spherePositionZ;

	spherePositionX.load_a(_positionX);
	spherePositionY.load_a(_positionY);
	spherePositionZ.load_a(_positionZ);

	Vec8f vX = rayPositionX - spherePositionX;
	Vec8f vY = rayPositionY - spherePositionY;
	Vec8f vZ = rayPositionZ - spherePositionZ;

	Vec8f rayDirectionX(ray.Direction.X);
	Vec8f rayDirectionY(ray.Direction.Y);
	Vec8f rayDirectionZ(ray.Direction.Z);

	Vec8f a = SimdDot(rayDirectionX, rayDirectionX, rayDirectionY, rayDirectionY, rayDirectionZ, rayDirectionZ);
	Vec8f b = SimdDot(vX, rayDirectionX, vY, rayDirectionY, vZ, rayDirectionZ);

	Vec8f sphereRadius;
	sphereRadius.load_a(_radius);

	Vec8f vDotV = SimdDot(vX, vX, vY, vY, vZ, vZ);
	Vec8f c = vDotV - (sphereRadius * sphereRadius);

	Vec8f discriminant = (b * b) - (a * c);
	Vec8f discriminantSqrt = sqrt(discriminant);

	Vec8f reciprocalA = approx_recipr(a);
	Vec8f negativeB = -b;

	Vec8f exitDistance = (negativeB + discriminantSqrt) * reciprocalA;

	Vec8f entranceDistance = max((negativeB - discriminantSqrt) * reciprocalA, Vec8f(0));
	Vec8f clampedEntranceDistance = select(exitDistance >= Vec8f(0), entranceDistance, infinite8f());

	float minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
	int minimumIndex = horizontal_find_first(Vec8f(minimumEntranceDistance) == clampedEntranceDistance);

	return SphereSoaIntersectionResult(_spheres[minimumIndex == -1 ? 0 : minimumIndex], minimumEntranceDistance);
}