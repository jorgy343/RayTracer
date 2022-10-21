export module RayMarcher;

import "Common.h";

import Geometry;
import Math;
import SignedDistance;
import SignedDistanceResult;

namespace Yart
{
    export constexpr real RayMarcherHitDistance = 0.0001;
    export constexpr real RayMarcherMaxDistance = 100;
    export constexpr unsigned int RayMarcherMaxSteps = 300;
    export constexpr real NormalEpsilon = 0.0001;

    export class RayMarcher : public Geometry
    {
    private:
        Vector3 SampleXCoordinates{NormalEpsilon, -NormalEpsilon, -NormalEpsilon};
        Vector3 SampleYCoordinates{-NormalEpsilon, NormalEpsilon, -NormalEpsilon};
        Vector3 SampleZCoordinates{-NormalEpsilon, -NormalEpsilon, NormalEpsilon};
        Vector3 SampleAllCoordinates{NormalEpsilon, NormalEpsilon, NormalEpsilon};

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
            Vector3 sampleX = SampleXCoordinates * std::get<0>(ClosestDistance(hitPosition + SampleXCoordinates)).Distance;
            Vector3 sampleY = SampleYCoordinates * std::get<0>(ClosestDistance(hitPosition + SampleYCoordinates)).Distance;
            Vector3 sampleZ = SampleZCoordinates * std::get<0>(ClosestDistance(hitPosition + SampleZCoordinates)).Distance;
            Vector3 sampleAll = SampleAllCoordinates * std::get<0>(ClosestDistance(hitPosition + SampleAllCoordinates)).Distance;

            return (sampleX + sampleY + sampleZ + sampleAll).Normalize();
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

            SignedDistanceResult closestDistance{std::numeric_limits<real>::infinity(), real{0}};
            const SignedDistance* closestChild{nullptr};

            for (unsigned int i = 0; i < RayMarcherMaxSteps; i++)
            {
                auto [distance, child] = ClosestDistance(point);

                closestDistance = distance;
                closestChild = child;

                if (distance.Distance == std::numeric_limits<real>::infinity() || distance.Distance >= RayMarcherMaxDistance)
                {
                    return IntersectionResult{nullptr, std::numeric_limits<real>::infinity()};
                }

                point += distance.Distance * ray.Direction;

                if (distance.Distance <= RayMarcherHitDistance)
                {
                    break;
                }
            }

            real totalDistance = Vector3::Distance(point, ray.Position);
            return IntersectionResult{this, totalDistance, closestDistance.MixAmount, closestChild == nullptr ? nullptr : closestChild->GetMaterial()};
        }

        std::tuple<SignedDistanceResult, const SignedDistance*> ClosestDistance(const Vector3& point) const
        {
            SignedDistanceResult closestDistance{std::numeric_limits<real>::infinity(), real{0}};
            const SignedDistance* closestChild{nullptr};

            for (const auto& child : Children)
            {
                SignedDistanceResult distance = child->ClosestDistance(point);
                if (distance.Distance < closestDistance.Distance)
                {
                    closestDistance = distance;
                    closestChild = child;
                }
            }

            return std::make_tuple(closestDistance, closestChild);
        }
    };
}