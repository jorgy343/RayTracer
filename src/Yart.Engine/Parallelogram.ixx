export module Parallelogram;

import "Common.h";

import AreaLight;
import BoundingBox;
import IntersectionResult;
import IntersectionResultType;
import Material;
import Math;
import Scene;

namespace Yart
{
    export class alignas(32) Parallelogram : public AreaLight
    {
    public:
        Vector3 Position{};
        Vector3 Edge1{};
        Vector3 Edge2{};
        Vector3 Normal{};
        real Area{};
        const Material* AppliedMaterial{nullptr};

    public:
        inline constexpr Parallelogram() = default;

        inline constexpr Parallelogram(const Vector3& position, const Vector3& edge1, const Vector3& edge2, const Material* appliedMaterial)
            : Position{position}, Edge1{edge1}, Edge2{edge2}, Normal{(edge1 % edge2).Normalize()}, AppliedMaterial{appliedMaterial}, Area{(edge1 % edge2).Length()}
        {

        }

        constexpr BoundingBox CalculateBoundingBox() const override
        {
            Vector3 point1 = Position + Edge1;
            Vector3 point2 = Position + Edge2;
            Vector3 point3 = Position + Edge1 + Edge2;

            return BoundingBox{
                Vector3::Min(Position, Vector3::Min(point1, Vector3::Min(point2, point3))),
                Vector3::Max(Position, Vector3::Max(point1, Vector3::Max(point2, point3))),
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
            Vector3 p = ray.Direction % Edge2;
            real determinant = Edge1 * p;

            if (Math::abs(determinant) < real{0.0})
            {
                return std::numeric_limits<real>::infinity();
            }

            real invDeterminant = Math::rcp(determinant);

            Vector3 t = ray.Position - Position;
            real a = (t * p) * invDeterminant;

            if (a < real{0.0} || a > real{1.0})
            {
                return std::numeric_limits<real>::infinity();
            }

            Vector3 q = t % Edge1;
            real b = (ray.Direction * q) * invDeterminant;

            if (b < real{0.0} || b > real{1.0})
            {
                return std::numeric_limits<real>::infinity();
            }

            real entranceDistance = (Edge2 * q) * invDeterminant;
            return entranceDistance >= real{0.0} ? entranceDistance : std::numeric_limits<real>::infinity();
        }

        inline Vector3 GetDirectionTowardsLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            Vector3 randomPointOnLight = GetPointOnLight(random, hitPosition, hitNormal);
            Vector3 directionToLight = (randomPointOnLight - hitPosition).Normalize();

            return directionToLight;
        }

        inline Vector3 GetPointOnLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            Vector3 randomPointOnLight = Position + (Edge1 * random.GetNormalized()) + (Edge2 * random.GetNormalized());
            return randomPointOnLight;
        }

        inline bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& positionOnLight) const override
        {
            Vector3 directionToLight = positionOnLight - hitPosition;
            real distanceToLight = directionToLight.Length();

            directionToLight.Normalize();

            Ray ray{hitPosition, directionToLight};
            real distance = scene.CastRayDistance(ray);

            return distance >= distanceToLight - real{0.01} ? false : true;
        }

        inline constexpr real CalculateInversePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection, const Vector3& outgoingDirection) const override
        {
            return Area;
        }
    };
}