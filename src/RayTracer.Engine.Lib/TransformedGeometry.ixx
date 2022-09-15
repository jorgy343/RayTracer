export module TransformedGeometry;

import "Common.h";

import Geometry;
import IntersectionResultType;
import Material;
import Matrix4x4;
import Ray;
import Vector3;
import Vector4;

namespace RayTracer
{
    export class TransformedGeometry final : public Geometry
    {
    public:
        const Geometry* ChildGeometry;
        Matrix4x4 InversedTransform;
        Matrix4x4 InverseTransposedTransform;

        constexpr TransformedGeometry(const Geometry* childGeometry, const Matrix4x4& transform)
            : ChildGeometry{childGeometry}, InversedTransform{transform.InvertConst()}, InverseTransposedTransform{transform.InvertConst().TransposeConst()}
        {

        }

        constexpr const Material* GetMaterial() const override final
        {
            return ChildGeometry->GetMaterial();
        }

        constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override final
        {
            Vector3 hitNormal = ChildGeometry->CalculateNormal(ray, hitPosition);
            Vector4 transformedHitNormal = Vector4{hitNormal, 0.0f} * InverseTransposedTransform;

            return Vector3{transformedHitNormal.X, transformedHitNormal.Y, transformedHitNormal.Z}.Normalize();
        }

        inline constexpr IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            return {this, Intersect<IntersectionResultType::Entrance>(ray)};
        }

        inline constexpr IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            return {this, Intersect<IntersectionResultType::Exit>(ray)};
        }

        template <IntersectionResultType TIntersectionResultType>
        force_inline constexpr float Intersect(const Ray& ray) const
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