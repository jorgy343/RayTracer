export module Cylinder;

import <limits>;
import <tuple>;

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

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, float additionalData) const override
        {
            Vector3 direction = (End - Start).Normalize();
            if (additionalData == std::numeric_limits<float>::infinity())
            {
                return direction;
            }
            else if (additionalData == -std::numeric_limits<float>::infinity())
            {
                return -direction;
            }
            else
            {
                Vector3 pointOnAxis = Start + direction * additionalData;
                Vector3 normal = (hitPosition - pointOnAxis).Normalize();

                return normal;
            }
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            auto [distance, additionalData] = Intersect<IntersectionResultType::Entrance>(ray);
            return {this, distance, additionalData};
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            auto [distance, additionalData] = Intersect< IntersectionResultType::Exit>(ray);
            return {this, distance, additionalData};
        }

        template <IntersectionResultType TIntersectionResultType>
        force_inline constexpr std::tuple<float, float> Intersect(const Ray& ray) const
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
                return std::make_tuple(std::numeric_limits<float>::infinity(), 0.0f);
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

            if (exitDistance >= 0.0f && y > 0.0f && y < caca)
            {
                return std::make_tuple(result, y);
            }
            else
            {
                // TODO: Remove this return statement when the caps work.
                return std::make_tuple(std::numeric_limits<float>::infinity(), 0.0f);

                // caps
                float distance = ((y < 0.0f ? 0.0f : caca) - caoc) / card;
                if (Math::abs(b + a * distance) < discriminantSqrt)
                {
                    float additionalData = y >= 0.0f ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
                    return std::make_tuple(distance, additionalData);
                }
                else
                {
                    return std::make_tuple(std::numeric_limits<float>::infinity(), 0.0f);
                }
            }
        }
    };
}