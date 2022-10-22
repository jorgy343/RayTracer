export module Sphere;

import "Common.h";

import BoundingBox;
import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Ray;
import SignedDistance;
import SignedDistanceResult;

namespace Yart
{
    export class __declspec(dllexport) alignas(32) Sphere : virtual public Geometry, virtual public SignedDistance
    {
    public:
        Vector3 Position{};
        real Radius{};
        const Material* AppliedMaterial{nullptr};

        Sphere() = default;

        Sphere(const Vector3& position, real radius, const Material* appliedMaterial)
            : Position{position}, Radius{radius}, AppliedMaterial{appliedMaterial}
        {

        }

        virtual BoundingBox CalculateBoundingBox() const override
        {
            return BoundingBox{
                Position - Radius,
                Position + Radius,
            };
        }

        virtual const Material* GetMaterial() const override
        {
            return AppliedMaterial;
        }

        virtual Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, real additionalData) const override
        {
            return (hitPosition - Position).Normalize();
        }

        virtual IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return {this, Intersect<IntersectionResultType::Entrance>(ray)};
        }

        virtual IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return {this, Intersect<IntersectionResultType::Exit>(ray)};
        }

        template <IntersectionResultType TIntersectionResultType>
        force_inline real Intersect(const Ray& ray) const
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

        virtual SignedDistanceResult ClosestDistance(const Vector3& point) const override
        {
            return {Vector3::Distance(point, Position) - Radius, real{0}};
        }
    };
}