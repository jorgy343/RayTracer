#pragma once

#include "Vcl.h"
#include "Alignment.h"
#include "Sphere.h"
#include "IntersectionResult.h"

namespace RayTracer
{
	class SphereSoa
	{
	private:
		AlignedVector<float, 64> _positionX;
		AlignedVector<float, 64> _positionY;
		AlignedVector<float, 64> _positionZ;
		AlignedVector<float, 64> _radius;
		AlignedVector<const Sphere*, 64> _spheres;

		inline IntersectionResult<Sphere> IntersectSoa(const Ray& ray, int sphereIndex) const;

	public:
		SphereSoa(int initialSize = 32);

		char GetCount() const;
		void AddSphere(const Sphere* sphere);

		IntersectionResult<Sphere> Intersect(const Ray& ray) const;
	};
}