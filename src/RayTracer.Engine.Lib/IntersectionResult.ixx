module;

#include <concepts>

export module RayTracer.IntersectionResult;

import RayTracer.Vector3;

namespace RayTracer
{
    export class Geometry;

    export class IntersectionResult
    {
    public:
        const Geometry* HitGeometry{nullptr};
        float Distance{0.0f};

        inline constexpr IntersectionResult(const Geometry* hitGeometry, float distance)
            : HitGeometry{hitGeometry}, Distance{distance}
        {

        }
    };
}