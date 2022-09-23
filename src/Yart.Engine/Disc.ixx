module;

export module Disc;

import <limits>;

import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Ray;
import Vector3;

namespace Yart
{
    export class __declspec(dllexport) alignas(16) Disc final : public Geometry
    {
    public:
        Vector3 Position{};
        Vector3 Normal{};
        float Distance{};
        float RadiusSquared{};

        const Material* AppliedMaterial{nullptr};

        inline constexpr Disc() = default;

        inline constexpr Disc(
            const Vector3& position,
            const Vector3& normal,
            const float radius,
            const Material* appliedMaterial)
            :
            Position{position},
            Normal{normal},
            Distance{-(normal * position)},
            RadiusSquared{radius * radius},
            AppliedMaterial{appliedMaterial}
        {

        }

        inline constexpr const Material* GetMaterial() const override final
        {
            return AppliedMaterial;
        }

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override final
        {
            return (ray.Direction * Normal) < 0.0f ? Normal : -Normal;
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            return {this, Intersect(ray)};
        }

        IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            return {this, Intersect(ray)};
        }

        inline constexpr float Intersect(const Ray& ray) const
        {
            float normalDotDirection = Normal * ray.Direction;
            float normalDotRayPosition = Normal * ray.Position;

            float entranceDistance = -(Distance + normalDotRayPosition) * Math::rcp(normalDotDirection);

            if (entranceDistance < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            Vector3 hitPosition = ray.Position + entranceDistance * ray.Direction;
            float distanceToCenterSquared = hitPosition.DistanceSquared(Position);

            return distanceToCenterSquared <= RadiusSquared ? entranceDistance : std::numeric_limits<float>::infinity();
        }
    };
}