module;

export module Parallelogram;

import <limits>;

import AreaLight;
import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Vector3;

namespace RayTracer
{
    export class alignas(16) Parallelogram : public Geometry, public AreaLight
    {
    public:
        Vector3 Position{};
        Vector3 Edge1{};
        Vector3 Edge2{};
        Vector3 Normal{};
        const Material* AppliedMaterial{nullptr};
        float Area{};

    public:
        inline constexpr Parallelogram() = default;

        inline constexpr Parallelogram(const Vector3& position, const Vector3& edge1, const Vector3& edge2, const Material* appliedMaterial)
            : Position{position}, Edge1{edge1}, Edge2{edge2}, Normal{(edge1 % edge2).Normalize()}, AppliedMaterial{appliedMaterial}, Area{(edge1 % edge2).Length()}
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

        constexpr IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            return {this, Intersect<IntersectionResultType::Entrance>(ray)};
        }

        constexpr IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            return {this, Intersect<IntersectionResultType::Exit>(ray)};
        }

        template <IntersectionResultType TIntersectionResultType>
        inline constexpr float Intersect(const Ray& ray) const
        {
            Vector3 p = ray.Direction % Edge2;
            float determinant = Edge1 * p;

            if (Math::abs(determinant) < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            float invDeterminant = Math::rcp(determinant);

            Vector3 t = ray.Position - Position;
            float a = (t * p) * invDeterminant;

            if (a < 0.0f || a > 1.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            Vector3 q = t % Edge1;
            float b = (ray.Direction * q) * invDeterminant;

            if (b < 0.0f || b > 1.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            float entranceDistance = (Edge2 * q) * invDeterminant;
            return entranceDistance >= 0.0f ? entranceDistance : std::numeric_limits<float>::infinity();
        }

        inline constexpr Vector3 GenerateRandomDirectionTowardsLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            Vector3 randomPointOnLight = Position + (Edge1 * random.GetNormalizedFloat()) + (Edge2 * random.GetNormalizedFloat());
            Vector3 directionToLight = (randomPointOnLight - hitPosition).Normalize();

            return directionToLight;
        }

        inline constexpr float CalculateInversePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection, const Vector3& outgoingDirection) const override
        {
            return Area;
        }
    };
}