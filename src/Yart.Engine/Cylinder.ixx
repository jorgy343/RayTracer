export module Cylinder;

import <limits>;

import "Common.h";

import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Vector3;

namespace Yart
{
    export class alignas(16) Cylinder : public Geometry
    {
    public:
        Vector3 Position{};
        Vector3 Direction{};
        float Height{};
        float Radius{};
        const Material* AppliedMaterial{nullptr};

        inline constexpr Cylinder() = default;

        inline constexpr Cylinder(
            const Vector3& position,
            const Vector3& direction,
            float height,
            float radius,
            const Material* appliedMaterial)
            :
            Position{position},
            Direction{direction},
            Height{height},
            Radius{radius},
            AppliedMaterial{appliedMaterial}
        {

        }

        inline constexpr const Material* GetMaterial() const override
        {
            return AppliedMaterial;
        }

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override
        {
            Vector3 axisLine = Position + Direction * Height;
            Vector3 v1 = hitPosition - Position;
            Vector3 v2 = v1.ProjectOnto(Direction) + Position;

            Vector3 normal = (v1 - v2).Normalize();
            return normal;
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return {this, Intersect<IntersectionResultType::Entrance>(ray)};
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return {this, Intersect< IntersectionResultType::Exit>(ray)};
        }

        template <IntersectionResultType TIntersectionResultType>
        force_inline constexpr float Intersect(const Ray& ray) const
        {
            Vector3 rayPosition = ray.Position - (Direction * ray.Position) * Direction;
            Vector3 rayDirection = ray.Direction - (Direction * ray.Direction) * Direction;

            Vector3 cylinderPosition = Position - (Direction * Position) * Direction;

            float a = rayDirection * rayDirection;
            float b = 2 * ((rayPosition * rayDirection) - (cylinderPosition * rayDirection));
            float c = -2 * (rayPosition * cylinderPosition) + (rayPosition * rayPosition) + (cylinderPosition * cylinderPosition) - Radius * Radius;

            Vector3 point;
            Vector3 positionToPoint;
            Vector3 pointProjectedOntoDirection;

            float radicand = b * b - 4 * a * c;
            if (radicand < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            float denominator = 2 * a;
            float negativeB = -b;

            float tMin = (negativeB - Math::sqrt(radicand)) / denominator;
            float tMax = (negativeB + Math::sqrt(radicand)) / denominator;

            if (tMax < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            point = ray.Position + tMin * ray.Direction;
            positionToPoint = point - Position;
            pointProjectedOntoDirection = positionToPoint.ProjectOnto(Direction);

            float cosAngle = (positionToPoint * Direction) / (positionToPoint.Length() * Direction.Length());
            if (positionToPoint * Direction <= 0.0f || pointProjectedOntoDirection.Length() > Height)
            {
                return std::numeric_limits<float>::infinity();
            }

            float result;
            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                result = tMin;
            }
            else
            {
                result = tMax;
            }

            return result;
        }
    };
}