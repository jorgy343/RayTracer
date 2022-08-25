#pragma once

#include "Vector3.h"
#include "Ray.h"

namespace RayTracer
{
	class alignas(16) Sphere
	{
	public:
		Vector3 Position;

	private:
		[[maybe_unused]] float _padding;

	public:
		float Radius;

		Sphere();
		Sphere(const Vector3& position, float radius);

		float Intersect(const Ray& ray) const;
	};
}