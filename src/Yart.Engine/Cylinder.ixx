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
        Vector3 Start{};
        Vector3 End{};
        float Radius{};
        const Material* AppliedMaterial{nullptr};

        inline constexpr Cylinder() = default;

        inline constexpr Cylinder(
            const Vector3& start,
            const Vector3& end,
            float radius,
            const Material* appliedMaterial)
            :
            Start{start},
            End{end},
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
            Vector3 direction = (End - Start).Normalize();
            float height = (End - Start).Length();

            Vector3 axisLine = Start + direction * height;
            Vector3 v1 = hitPosition - Start;
            Vector3 v2 = v1.ProjectOnto(direction) + Start;

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
            Vector3 ca = End - Start;
            Vector3 oc = ray.Position - Start;

            float caca = ca * ca;
            float card = ca * ray.Direction;
            float caoc = ca * oc;

            float a = caca - card * card;
            float b = caca * (oc * ray.Direction) - caoc * card;
            float c = caca * (oc * oc) - caoc * caoc - Radius * Radius * caca;

            float discriminant = b * b - a * c;
            if (discriminant < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            float discriminantSqrt = Math::sqrt(discriminant);
            
            float inverseA = Math::rcp(a);
            float exitDistance = (-b + discriminantSqrt) * inverseA;

            // body
            float result;
            float y;

            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                float entranceDistance = (-b - discriminantSqrt) * inverseA;

                y = caoc + entranceDistance * card;
                result = entranceDistance;
            }
            else
            {
                y = caoc + exitDistance * card;
                result = exitDistance;
            }

            return exitDistance >= 0.0f && y > 0.0f && y < caca ? result : std::numeric_limits<float>::infinity();
            // Normal = (hitPosition - ca * y / caca) / Radius




            // caps
            //t = (((y < 0.0) ? 0.0 : caca) - caoc) / card;
            //if (Math::abs(b + a * t) < h)
            //{
            //    return t;

            //    // Normal = ca * sign(y) / caca
            //}
        }
    };
}