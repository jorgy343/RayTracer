export module Cylinder;

import "Common.h";

import Geometry;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;

namespace Yart
{
    export class alignas(32) Cylinder : public Geometry
    {
    public:
        Vector3 Start{};
        Vector3 End{};
        real Radius{};
        const Material* AppliedMaterial{nullptr};

        inline constexpr Cylinder() = default;

        inline constexpr Cylinder(
            const Vector3& start,
            const Vector3& end,
            real radius,
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

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, real additionalData) const override
        {
            Vector3 direction = (End - Start).Normalize();
            if (additionalData == std::numeric_limits<real>::infinity())
            {
                return direction;
            }
            else if (additionalData == -std::numeric_limits<real>::infinity())
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
        force_inline constexpr std::tuple<real, real> Intersect(const Ray& ray) const
        {
            Vector3 ca = End - Start;
            Vector3 oc = ray.Position - Start;

            real caca = ca * ca;
            real card = ca * ray.Direction;
            real caoc = ca * oc;

            real a = caca - card * card;
            real b = caca * (oc * ray.Direction) - caoc * card;
            real c = caca * (oc * oc) - caoc * caoc - Radius * Radius * caca;

            real discriminant = b * b - a * c;
            if (discriminant < real{0.0})
            {
                return std::make_tuple(std::numeric_limits<real>::infinity(), real{0.0});
            }

            real discriminantSqrt = Math::sqrt(discriminant);
            
            real inverseA = Math::rcp(a);
            real exitDistance = (-b + discriminantSqrt) * inverseA;

            // body
            real result;
            real y;

            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                real entranceDistance = (-b - discriminantSqrt) * inverseA;

                y = caoc + entranceDistance * card;
                result = entranceDistance;
            }
            else
            {
                y = caoc + exitDistance * card;
                result = exitDistance;
            }

            if (exitDistance >= real{0.0} && y > real{0.0} && y < caca)
            {
                return std::make_tuple(result, y);
            }
            else
            {
                // TODO: Remove this return statement when the caps work.
                return std::make_tuple(std::numeric_limits<real>::infinity(), real{0.0});

                // caps
                real distance = ((y < real{0.0} ? real{0.0} : caca) - caoc) / card;
                if (Math::abs(b + a * distance) < discriminantSqrt)
                {
                    real additionalData = y >= real{0.0} ? std::numeric_limits<real>::infinity() : -std::numeric_limits<real>::infinity();
                    return std::make_tuple(distance, additionalData);
                }
                else
                {
                    return std::make_tuple(std::numeric_limits<real>::infinity(), real{0.0});
                }
            }
        }
    };
}