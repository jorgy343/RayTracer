module;

export module IntersectionResult;

import <concepts>;

import Vector3;

namespace RayTracer
{
    export class Geometry;
    export class IntersectionResult;

    IntersectionResult Test();

    export class IntersectionResult
    {
    public:
        const Geometry* HitGeometry{nullptr};
        float HitDistance{0.0f};

        inline constexpr IntersectionResult(const Geometry* hitGeometry, float hitDistance)
            : HitGeometry{hitGeometry}, HitDistance{hitDistance}
        {

        }
    };
}