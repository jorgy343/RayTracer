module;

export module Sphere;

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
    export class __declspec(dllexport) alignas(16) Sphere final : public Geometry
    {
    public:
        Vector3 Position{};
        float Radius{0.0f};
        std::shared_ptr<const Material> AppliedMaterial{nullptr};

        inline constexpr Sphere() = default;

        inline Sphere(const Vector3& position, float radius, std::shared_ptr<const Material> appliedMaterial)
            : Position{position}, Radius{radius}, AppliedMaterial{appliedMaterial}
        {

        }

        inline const Material* GetMaterial() const override final
        {
            return AppliedMaterial.get();
        }

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override final
        {
            return (hitPosition - Position).Normalize();
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
            Vector3 v = ray.Position - Position;

            float a = ray.Direction * ray.Direction;
            float b = v * ray.Direction;
            float c = (v * v) - (Radius * Radius);

            float discriminant = (b * b) - (a * c);
            float discriminantSqrt = Math::sqrt(discriminant);

            float reciprocalA = Math::rcp(a);
            float negativeB = -b;

            float exitDistance = (negativeB + discriminantSqrt) * reciprocalA;

            float result;
            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                float entranceDistance = (negativeB - discriminantSqrt) * reciprocalA;
                result = entranceDistance;
            }
            else
            {
                result = exitDistance;
            }

            return exitDistance >= 0.0f ? result : std::numeric_limits<float>::infinity();
        }
    };
}