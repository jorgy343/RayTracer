export module Sphere;

import <limits>;

import BoundingBox;
import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Ray;

namespace Yart
{
    export class __declspec(dllexport) alignas(16) Sphere : public Geometry
    {
    public:
        Vector3 Position{};
        real Radius{real{0.0}};
        const Material* AppliedMaterial{nullptr};

        inline constexpr Sphere() = default;

        inline constexpr Sphere(const Vector3& position, real radius, const Material* appliedMaterial)
            : Position{position}, Radius{radius}, AppliedMaterial{appliedMaterial}
        {

        }

        constexpr BoundingBox CalculateBoundingBox() const override
        {
            return BoundingBox{
                Position - Radius,
                Position + Radius,
            };
        }

        inline constexpr const Material* GetMaterial() const override
        {
            return AppliedMaterial;
        }

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, real additionalData) const override
        {
            return (hitPosition - Position).Normalize();
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return {this, Intersect<IntersectionResultType::Entrance>(ray)};
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return {this, Intersect<IntersectionResultType::Exit>(ray)};
        }

        template <IntersectionResultType TIntersectionResultType>
        inline constexpr real Intersect(const Ray& ray) const
        {
            Vector3 v = ray.Position - Position;

            real a = ray.Direction * ray.Direction;
            real b = v * ray.Direction;
            real c = (v * v) - (Radius * Radius);

            real discriminant = (b * b) - (a * c);
            if (discriminant < real{0.0})
            {
                return std::numeric_limits<real>::infinity();
            }

            real discriminantSqrt = Math::sqrt(discriminant);

            real reciprocalA = Math::rcp(a);
            real negativeB = -b;

            real exitDistance = (negativeB + discriminantSqrt) * reciprocalA;

            real result;
            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                real entranceDistance = (negativeB - discriminantSqrt) * reciprocalA;
                result = entranceDistance;
            }
            else
            {
                result = exitDistance;
            }

            return exitDistance >= real{0.0} ? result : std::numeric_limits<real>::infinity();
        }
    };
}