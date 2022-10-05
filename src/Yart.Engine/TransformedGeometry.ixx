export module TransformedGeometry;

import "Common.h";

import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Ray;

namespace Yart
{
	export class TransformedGeometry : public Geometry
	{
	protected:
		const Geometry* ChildGeometry{nullptr};
		Matrix4x4 InversedTransform{};
		Matrix4x4 InverseTransposedTransform{};

	public:
		inline constexpr TransformedGeometry(const Geometry* childGeometry, const Matrix4x4& transform)
			:
			ChildGeometry{childGeometry},
			InversedTransform{transform.InvertConst()},
			InverseTransposedTransform{transform.InvertConst().TransposeConst()}
		{

		}

		inline constexpr const Material* GetMaterial() const override
		{
			return ChildGeometry->GetMaterial();
		}

		constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, float additionalData) const override
		{
			Vector3 hitNormal = ChildGeometry->CalculateNormal(ray, hitPosition, additionalData);
			Vector4 transformedHitNormal = Vector4{hitNormal, 0.0f} *InverseTransposedTransform;

			return Vector3{transformedHitNormal.X, transformedHitNormal.Y, transformedHitNormal.Z}.Normalize();
		}

		inline IntersectionResult IntersectEntrance(const Ray& ray) const override
		{
			return {this, Intersect<IntersectionResultType::Entrance>(ray)};
		}

		inline IntersectionResult IntersectExit(const Ray& ray) const override
		{
			return {this, Intersect<IntersectionResultType::Exit>(ray)};
		}

		template <IntersectionResultType TIntersectionResultType>
		force_inline float Intersect(const Ray& ray) const
		{
			Vector4 transformedPosition = Vector4{ray.Position, 1.0f} *InversedTransform;
			Vector4 transformedDirection = Vector4{ray.Direction, 0.0f} *InversedTransform;

			Ray transformedRay
			{
				Vector3{transformedPosition.X, transformedPosition.Y, transformedPosition.Z},
				Vector3{transformedDirection.X, transformedDirection.Y, transformedDirection.Z},
			};

			if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
			{
				return ChildGeometry->IntersectEntrance(transformedRay).HitDistance;
			}
			else
			{
				return ChildGeometry->IntersectExit(transformedRay).HitDistance;
			}
		}
	};
}