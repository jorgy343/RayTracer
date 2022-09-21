module;

export module Plane;

import <limits>;
import <memory>;

import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Ray;
import Vector3;

namespace Yart
{
    export class __declspec(dllexport) alignas(16) Plane final : public Geometry
    {
    public:
        Vector3 Normal{};
        float Distance{0.0f};
        std::shared_ptr<const Material> AppliedMaterial{nullptr};

        inline constexpr Plane() = default;

        inline Plane(const Vector3& normal, float distance, std::shared_ptr<const Material> appliedMaterial)
            : Normal{normal}, Distance{distance}, AppliedMaterial{appliedMaterial}
        {

        }

        inline Plane(const Vector3& normal, const Vector3& point, std::shared_ptr<const Material> appliedMaterial)
            : Normal{normal}, Distance{-(normal * point)}, AppliedMaterial{appliedMaterial}
        {

        }

        inline const Material* GetMaterial() const override final
        {
            return AppliedMaterial.get();
        }

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override final
        {
            return (ray.Direction * Normal) < 0.0f ? Normal : -Normal;
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            return {this, Intersect<IntersectionResultType::Entrance>(ray)};
        }

        IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            return {this, Intersect<IntersectionResultType::Exit>(ray)};
        }

        template <IntersectionResultType TIntersectionResultType>
        inline float Intersect(const Ray& ray) const
        {
            float normalDotDirection = Normal * ray.Direction;
            float normalDotRayPosition = Normal * ray.Position;

            float entranceDistance = -(Distance + normalDotRayPosition) * Math::rcp(normalDotDirection);

            return entranceDistance >= 0.0f ? entranceDistance : std::numeric_limits<float>::infinity();
        }
    };
}