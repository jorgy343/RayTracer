#pragma once

#include <limits>

#include "Vcl.h"
#include "Sphere.h"

namespace RayTracer
{
	class SphereSoaIntersectionResult
	{
	public:
		const Sphere* Sphere;
		float Distance;

		SphereSoaIntersectionResult(const RayTracer::Sphere* sphere, float distance);
	};

	class alignas(64) SphereSoa
	{
	private:
		float _positionX[8];
		float _positionY[8];
		float _positionZ[8];
		float _radius[8];
		const Sphere* _spheres[8];
		char _count;

	public:
		SphereSoa();

		void Reset();
		void Finalize();
		char GetCount() const;

		void AddSphere(const Sphere* sphere);
		SphereSoaIntersectionResult Intersect(const Ray& ray) const;
	};
}