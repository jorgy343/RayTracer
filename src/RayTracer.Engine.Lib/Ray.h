#pragma once

#include "Vector3.h"

namespace RayTracer
{
	class alignas(16) Ray
	{
	public:
		Vector3 Position;
		Vector3 Direction;

		Ray();
		Ray(const Vector3& position, const Vector3& direction);
	};
}