#include <concepts>

export module RayTracer.IntersectionResult;

import RayTracer.Geometry;
import RayTracer.Vector3;

namespace RayTracer
{
    export class IntersectionResult
    {
    public:
        const Geometry* HitGeometry{nullptr};
        float Distance{0.0f};

        IntersectionResult(const Geometry* hitGeometry, float distance)
            : HitGeometry{hitGeometry}, Distance{distance}
        {

        }
    };
}