export module Plane;

import "Common.h";

import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Ray;

namespace Yart
{
    export class __declspec(dllexport) alignas(32) Plane : public Geometry
    {
    public:
        Vector3 Normal{};
        real Distance{};
        const Material* AppliedMaterial{nullptr};

        inline constexpr Plane() = default;

        inline constexpr Plane(const Vector3& normal, real distance, const Material* appliedMaterial)
            : Normal{normal}, Distance{distance}, AppliedMaterial{appliedMaterial}
        {

        }

        inline constexpr Plane(const Vector3& normal, const Vector3& point, const Material* appliedMaterial)
            : Normal{normal}, Distance{-(normal * point)}, AppliedMaterial{appliedMaterial}
        {

        }

        inline constexpr const Material* GetMaterial() const override
        {
            return AppliedMaterial;
        }

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, real additionalData) const override
        {
            return (ray.Direction * Normal) < real{0.0} ? Normal : -Normal;
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return {this, Intersect(ray)};
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return {this, Intersect(ray)};
        }

        inline constexpr real Intersect(const Ray& ray) const
        {
            real normalDotDirection = Normal * ray.Direction;
            real normalDotRayPosition = Normal * ray.Position;

            real entranceDistance = -(Distance + normalDotRayPosition) * Math::rcp(normalDotDirection);

            return entranceDistance >= real{0.0} ? entranceDistance : std::numeric_limits<real>::infinity();
        }
    };
}