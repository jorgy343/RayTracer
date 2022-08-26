export module RayTracer.IntersectionResult;

import RayTracer.Vector3;

namespace RayTracer
{
	export template<typename TShape>
	class IntersectionResult
	{
	public:
		const TShape* Shape;
		float Distance;
		Vector3 Normal;

		IntersectionResult(const TShape* shape, float distance, const Vector3& normal)
			: Shape{shape}, Distance{distance}, Normal{normal}
		{

		}
	};
}