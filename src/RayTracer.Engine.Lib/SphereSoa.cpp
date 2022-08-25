#include "SphereSoa.h"

#include <limits>

#include "Vcl.h"
#include "Simd.h"

using namespace RayTracer;
using namespace vcl;

SphereSoa::SphereSoa(int initialSize)
    : _positionX(), _positionY(), _positionZ(), _radius(), _spheres()
{
	_positionX.reserve(initialSize);
	_positionY.reserve(initialSize);
	_positionZ.reserve(initialSize);
	_radius.reserve(initialSize);
	_spheres.reserve(initialSize);
}

char SphereSoa::GetCount() const
{
    return _positionX.size();
}

void SphereSoa::AddSphere(const Sphere* sphere)
{
    _positionX.push_back(sphere->Position.X);
    _positionY.push_back(sphere->Position.Y);
    _positionZ.push_back(sphere->Position.Z);
    _radius.push_back(sphere->Radius);
    _spheres.push_back(sphere);
}

IntersectionResult<Sphere> SphereSoa::Intersect(const Ray& ray) const
{
    auto i = 0;
    auto result = IntersectionResult<Sphere>(nullptr, std::numeric_limits<float>::infinity(), Vector3(0.0f));

    for (; i + 8 <= _positionX.size(); i += 8)
    {
		auto multiSphereResult = IntersectSoa(ray, i);

		if (multiSphereResult.Distance < result.Distance)
		{
			result = multiSphereResult;
		}
    }

    for (; i < _positionX.size(); i++)
    {
        auto singleSphereResult = _spheres[i]->Intersect(ray);

        if (singleSphereResult < result.Distance)
        {
            result = IntersectionResult<Sphere>(nullptr, singleSphereResult, Vector3(0.0f));
        }
    }

    return result;
}

IntersectionResult<Sphere> SphereSoa::IntersectSoa(const Ray& ray, int sphereIndex) const
{
	Vec8f rayPositionX(ray.Position.X);
	Vec8f rayPositionY(ray.Position.Y);
	Vec8f rayPositionZ(ray.Position.Z);

	Vec8f spherePositionX;
	Vec8f spherePositionY;
	Vec8f spherePositionZ;

	spherePositionX.load_a(&_positionX[sphereIndex]);
	spherePositionY.load_a(&_positionY[sphereIndex]);
	spherePositionZ.load_a(&_positionZ[sphereIndex]);

	Vec8f vX = rayPositionX - spherePositionX;
	Vec8f vY = rayPositionY - spherePositionY;
	Vec8f vZ = rayPositionZ - spherePositionZ;

	Vec8f rayDirectionX(ray.Direction.X);
	Vec8f rayDirectionY(ray.Direction.Y);
	Vec8f rayDirectionZ(ray.Direction.Z);

	Vec8f a = SimdDot(rayDirectionX, rayDirectionX, rayDirectionY, rayDirectionY, rayDirectionZ, rayDirectionZ);
	Vec8f b = SimdDot(vX, rayDirectionX, vY, rayDirectionY, vZ, rayDirectionZ);

	Vec8f sphereRadius;
	sphereRadius.load_a(&_radius[sphereIndex]);

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

	return IntersectionResult<Sphere>(
		_spheres[sphereIndex + (minimumIndex == -1 ? 0 : minimumIndex)],
		minimumEntranceDistance,
		Vector3(0.0f));
}