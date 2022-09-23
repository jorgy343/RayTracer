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
    export template <bool IncludeCaps = true>
    class alignas(16) Cylinder : public Geometry
    {
    public:
        Vector3 Position{};
        Vector3 Direction{};
        float Height{};
        float Radius{};
        const Material* AppliedMaterial{nullptr};

        inline constexpr Cylinder() = default;

        inline constexpr Cylinder(
            const Vector3& position,
            const Vector3& direction,
            float height,
            float radius,
            const Material* appliedMaterial)
            :
            Position{position},
            Direction{direction},
            Height{height},
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
            return Vector3{};
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return {this, Intersect(ray)};
        }

        IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return {this, Intersect(ray)};
        }

        force_inline constexpr float Intersect(const Ray& ray) const
        {
            return std::numeric_limits<float>::infinity();
        //    Vector3 rayPosition = ray.Position - (Direction * ray.Position) * Direction;
        //    Vector3 rayDirection = ray.Direction - (Direction * ray.Direction) * Direction;

        //    Vector3 cylinderPosition = Position - (Direction * Position) * Direction;

        //    float a = rayDirection * rayDirection;
        //    float b = 2 * ((rayPosition * rayDirection) - (cylinderPosition * rayDirection));
        //    float c = -2 * (rayPosition * cylinderPosition) + (rayPosition * rayPosition) + (cylinderPosition * cylinderPosition) - Radius * Radius;

        //    float tMin = std::numeric_limits<float>::infinity();
        //    float tMax = std::numeric_limits<float>::infinity();

        //    bool isValid = true;

        //    Vector3 point;
        //    Vector3 positionToPoint;
        //    Vector3 pointProjectedOntoDirection;

        //    float radicand = b * b - 4 * a * c;
        //    if (radicand < 0.0f)
        //    {
        //        isValid = false;
        //        goto checkCaps;
        //    }

        //    float denominator = 2 * a;
        //    float negativeB = -b;

        //    tMin = (negativeB - Math::sqrt(radicand)) / denominator;
        //    tMax = (negativeB + Math::sqrt(radicand)) / denominator;

        //    if (tMax < 0.0f)
        //    {
        //        isValid = false;
        //        goto checkCaps;
        //    }

        //    point = ray.Position + tMin * ray.Direction;
        //    positionToPoint = point - Position;
        //    pointProjectedOntoDirection = Vector3::Project(positionToPoint, Direction);

        //    float cosAngle = Vector3::Dot(positionToPoint, Direction) / (positionToPoint.GetLength() * Direction.GetLength());
        //    if (Vector3::Dot(positionToPoint, Direction) <= 0.0f || pointProjectedOntoDirection.GetLength() > Height)
        //    {
        //        isValid = false;
        //        goto checkCaps;
        //    }

        //checkCaps:
        //    bool intersectionDataSet = false;

        //    IntersectionData bottomCapIntersectionData;
        //    if (IntersectBottomCap(ray, &bottomCapIntersectionData))
        //    {
        //        *intersectionData = bottomCapIntersectionData;
        //        intersectionDataSet = true;
        //    }

        //    IntersectionData topCapIntersectionData;
        //    if (IntersectTopCap(ray, &topCapIntersectionData) && (!intersectionDataSet || topCapIntersectionData.GetDistance() < intersectionData->GetDistance()))
        //    {
        //        *intersectionData = topCapIntersectionData;
        //        intersectionDataSet = true;
        //    }

        //    // Determine the point and normal of intersection.
        //    if (isValid && (!intersectionDataSet || tMin < intersectionData->GetDistance()))
        //    {
        //        Vector3 axisLine = Position + Direction * Height;
        //        Vector3 v1 = point - Position;
        //        Vector3 v2 = Vector3::Project(v1, Direction) + Position;

        //        Vector3 normal = (v1 - v2).Normalize();

        //        *intersectionData = IntersectionData(tMin, point, normal, this, this);
        //        intersectionDataSet = true;
        //    }

        //    if (intersectionDataSet)
        //    {
        //        return this;
        //    }

        //    return nullptr;








        //    return entranceDistance >= 0.0f ? entranceDistance : std::numeric_limits<float>::infinity();
        }
    };
}