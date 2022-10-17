export module Disc;

import "Common.h";

import AreaLight;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Ray;
import Scene;

namespace Yart
{
    export class __declspec(dllexport) alignas(32) Disc : public AreaLight
    {
    public:
        Vector3 Position{};
        Vector3 Normal{};
        real Distance{};
        real Radius{};
        real RadiusSquared{};
        real Area{};
        const Material* AppliedMaterial{nullptr};

        inline constexpr Disc() = default;

        inline constexpr Disc(
            const Vector3& position,
            const Vector3& normal,
            const real radius,
            const Material* appliedMaterial)
            :
            Position{position},
            Normal{normal},
            Distance{-(normal * position)},
            Radius{radius},
            RadiusSquared{radius * radius},
            Area{Pi * radius * radius},
            AppliedMaterial{appliedMaterial}
        {

        }

        // TODO: Bounding box code taken from the sphere version. Could optimize the bounding box to match the shape of a disc better.
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

            if (entranceDistance < real{0.0})
            {
                return std::numeric_limits<real>::infinity();
            }

            Vector3 hitPosition = ray.Position + entranceDistance * ray.Direction;
            real distanceToCenterSquared = Vector3::DistanceSquared(hitPosition, Position);

            return distanceToCenterSquared <= RadiusSquared ? entranceDistance : std::numeric_limits<real>::infinity();




            // Source: https://iquilezles.org/articles/intersectors/
            //Vector3 o = ray.Position - Position;
            //real t = -(Normal * o) / (ray.Direction * Normal);
            //Vector3 q = o + t * ray.Direction;

            //return q * q < RadiusSquared ? t : std::numeric_limits<real>::infinity();
        }

        inline Vector3 GetDirectionTowardsLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            Vector3 randomPointOnLight = GetPointOnLight(random, hitPosition, hitNormal);
            Vector3 directionToLight = (randomPointOnLight - hitPosition).Normalize();

            return directionToLight;
        }

        inline Vector3 GetPointOnLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            real distanceFromCenter = Radius * Math::sqrt(random.GetNormalized());
            real theta = random.GetNormalized() * TwoPi;

            real dx = distanceFromCenter * Math::cos(theta);
            real dy = distanceFromCenter * Math::sin(theta);

            Vector3 xDirection = Vector3::BuildPerpendicularVector(Normal);
            Vector3 yDirection = Normal % xDirection;

            Vector3 randomPointOnLight = Position + xDirection * dx + yDirection * dy;
            return randomPointOnLight;
        }

        inline bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& positionOnLight) const override
        {
            Vector3 directionToLight = positionOnLight - hitPosition;
            real distanceToLightSquared = directionToLight.LengthSquared();

            directionToLight.Normalize();

            Ray ray{hitPosition, directionToLight};
            real distance = scene.CastRayDistance(ray);

            return distance * distance >= distanceToLightSquared - real{0.01} ? false : true;
        }

        inline constexpr real CalculateInversePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection, const Vector3& outgoingDirection) const override
        {
            return Area;
        }
    };
}