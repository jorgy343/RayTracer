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
    export class __declspec(dllexport) alignas(16) Disc : public AreaLight
    {
    public:
        Vector3 Position{};
        Vector3 Normal{};
        float Distance{};
        float Radius{};
        float RadiusSquared{};
        float Area{};
        const Material* AppliedMaterial{nullptr};

        inline constexpr Disc() = default;

        inline constexpr Disc(
            const Vector3& position,
            const Vector3& normal,
            const float radius,
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

        inline constexpr const Material* GetMaterial() const override
        {
            return AppliedMaterial;
        }

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, float additionalData) const override
        {
            return (ray.Direction * Normal) < 0.0f ? Normal : -Normal;
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return {this, Intersect(ray)};
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return {this, Intersect(ray)};
        }

        inline constexpr float Intersect(const Ray& ray) const
        {
            float normalDotDirection = Normal * ray.Direction;
            float normalDotRayPosition = Normal * ray.Position;

            float entranceDistance = -(Distance + normalDotRayPosition) * Math::rcp(normalDotDirection);

            if (entranceDistance < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            Vector3 hitPosition = ray.Position + entranceDistance * ray.Direction;
            float distanceToCenterSquared = hitPosition.DistanceSquared(Position);

            return distanceToCenterSquared <= RadiusSquared ? entranceDistance : std::numeric_limits<float>::infinity();




            // Source: https://iquilezles.org/articles/intersectors/
            //Vector3 o = ray.Position - Position;
            //float t = -(Normal * o) / (ray.Direction * Normal);
            //Vector3 q = o + t * ray.Direction;

            //return q * q < RadiusSquared ? t : std::numeric_limits<float>::infinity();
        }

        inline Vector3 GetDirectionTowardsLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            Vector3 randomPointOnLight = GetPointOnLight(random, hitPosition, hitNormal);
            Vector3 directionToLight = (randomPointOnLight - hitPosition).Normalize();

            return directionToLight;
        }

        inline Vector3 GetPointOnLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            float distanceFromCenter = Radius * Math::sqrt(random.GetNormalizedFloat());
            float theta = random.GetNormalizedFloat() * TwoPi;

            float dx = distanceFromCenter * Math::cos(theta);
            float dy = distanceFromCenter * Math::sin(theta);

            Vector3 xDirection = Normal.BuildPerpendicularVector();
            Vector3 yDirection = Normal % xDirection;

            Vector3 randomPointOnLight = Position + xDirection * dx + yDirection * dy;
            return randomPointOnLight;
        }

        inline bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& positionOnLight) const override
        {
            Vector3 directionToLight = positionOnLight - hitPosition;
            float distanceToLightSquared = directionToLight.LengthSquared();

            directionToLight.Normalize();

            Ray ray{hitPosition, directionToLight};
            float distance = scene.CastRayDistance(ray);

            return distance * distance >= distanceToLightSquared - 0.01f ? false : true;
        }

        inline constexpr float CalculateInversePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection, const Vector3& outgoingDirection) const override
        {
            return Area;
        }
    };
}