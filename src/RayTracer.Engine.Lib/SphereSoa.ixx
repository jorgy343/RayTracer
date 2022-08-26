#include <cmath>

#include "Vcl.h"

export module RayTracer.SphereSoa;

import RayTracer.Alignment;
import RayTracer.IntersectionResult;
import RayTracer.Ray;
import RayTracer.Simd;
import RayTracer.Sphere;

using namespace vcl;

namespace RayTracer
{
	export class SphereSoa
	{
	private:
		AlignedVector<float, 64> _positionX{};
		AlignedVector<float, 64> _positionY{};
		AlignedVector<float, 64> _positionZ{};
		AlignedVector<float, 64> _radius{};
		AlignedVector<const Sphere*, 64> _spheres{};

	public:
		SphereSoa(int initialSize = 32)
		{
			_positionX.reserve(initialSize);
			_positionY.reserve(initialSize);
			_positionZ.reserve(initialSize);
			_radius.reserve(initialSize);
			_spheres.reserve(initialSize);
		}

		char GetCount() const
		{
			return _positionX.size();
		}

		void AddSphere(const Sphere* sphere)
		{
			_positionX.push_back(sphere->Position.X);
			_positionY.push_back(sphere->Position.Y);
			_positionZ.push_back(sphere->Position.Z);
			_radius.push_back(sphere->Radius);
			_spheres.push_back(sphere);
		}

		void Finalize()
		{
			for (auto i = _positionX.size(); i % 8 != 0; i++)
			{
				_positionX.push_back(std::numeric_limits<float>::infinity());
				_positionY.push_back(std::numeric_limits<float>::infinity());
				_positionZ.push_back(std::numeric_limits<float>::infinity());
				_radius.push_back(std::numeric_limits<float>::infinity());
				_spheres.push_back(nullptr);
			}
		}

		IntersectionResult<Sphere> Intersect(const Ray& ray) const
		{
			auto result = IntersectionResult<Sphere>(nullptr, std::numeric_limits<float>::infinity(), Vector3(0.0f));

			for (auto i = 0; i + 8 <= _positionX.size(); i += 8)
			{
				auto multiSphereResult = IntersectSoa(ray, i);

				if (multiSphereResult.Distance < result.Distance)
				{
					result = multiSphereResult;
				}
			}

			return result;
		}

	private:
		IntersectionResult<Sphere> IntersectSoa(const Ray& ray, int sphereIndex) const
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
	};
}