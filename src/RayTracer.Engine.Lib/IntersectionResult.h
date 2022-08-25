#pragma once

#include "Vector3.h"

namespace RayTracer
{
	template<typename TShape>
	class IntersectionResult
	{
	public:
		const TShape* Shape;
		float Distance;
		Vector3 Normal;

		IntersectionResult(const TShape* shape, float distance, const Vector3& normal)
			: Shape(shape), Distance(distance), Normal(normal)
		{

		}
	};
}