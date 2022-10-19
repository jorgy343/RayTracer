export module RayMarcher;

import "Common.h";

import Geometry;
import Math;
import SignedDistance;

namespace Yart
{
    export constexpr real RayMarcherHitDistance = 0.0001;
    export constexpr real RayMarcherMaxDistance = 100;
    export constexpr unsigned int RayMarcherMaxSteps = 300;
    export constexpr real NormalEpsilon = 0.0001;

    export class RayMarcher : public Geometry
    {
    protected:
        std::vector<const SignedDistance*> Children{};

    public:
        inline explicit RayMarcher(std::vector<const SignedDistance*> children)
            : Children{children}
        {

        }

        BoundingBoxT<real> CalculateBoundingBox() const override
        {
            BoundingBoxT<real> boundingBox = BoundingBoxT<real>::ReverseInfinity();

            for (const auto& child : Children)
            {
                boundingBox = boundingBox.Union(child->CalculateBoundingBox());
            }

            return boundingBox;
        }

        const Material* GetMaterial() const final override
        {
            return nullptr;
        }

        Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, real additionalData) const override
        {
            real distance = std::get<0>(ClosestDistance(hitPosition));

            Vector3 normal
            {
                std::get<0>(ClosestDistance(hitPosition + Vector3{NormalEpsilon, real{}, real{}})) - std::get<0>(ClosestDistance(hitPosition - Vector3{NormalEpsilon, real{}, real{}})),
                std::get<0>(ClosestDistance(hitPosition + Vector3{real{}, NormalEpsilon, real{}})) - std::get<0>(ClosestDistance(hitPosition - Vector3{real{}, NormalEpsilon, real{}})),
                std::get<0>(ClosestDistance(hitPosition + Vector3{real{}, real{}, NormalEpsilon})) - std::get<0>(ClosestDistance(hitPosition - Vector3{real{}, real{}, NormalEpsilon})),
            };

            return normal.Normalize();
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return Intersect(ray);
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return Intersect(ray);
        }

        force_inline IntersectionResult Intersect(const Ray& ray) const
        {
            Vector3 point = ray.Position;

            const SignedDistance* closestChild{nullptr};

            for (unsigned int i = 0; i < RayMarcherMaxSteps; i++)
            {
                auto [distance, child] = ClosestDistance(point);
                closestChild = child;

                if (distance == std::numeric_limits<real>::infinity() || distance >= RayMarcherMaxDistance)
                {
                    return IntersectionResult{nullptr, std::numeric_limits<real>::infinity()};
                }

                point += distance * ray.Direction;

                if (distance <= RayMarcherHitDistance)
                {
                    break;
                }
            }

            real distance = Vector3::Distance(point, ray.Position);
            return IntersectionResult{this, distance, distance, closestChild == nullptr ? nullptr : closestChild->AppliedMaterial};
        }

        std::tuple<real, const SignedDistance*> ClosestDistance(const Vector3& point) const
        {
            real closestDistance{std::numeric_limits<real>::infinity()};
            const SignedDistance* closestChild{nullptr};

            for (const auto& child : Children)
            {
                real distance = child->ClosestDistance(point);
                if (distance < closestDistance)
                {
                    closestDistance = distance;
                    closestChild = child;
                }
            }

            return std::make_tuple(closestDistance, closestChild);
        }
    };
}