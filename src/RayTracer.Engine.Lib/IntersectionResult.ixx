#include <concepts>

export module RayTracer.IntersectionResult;

import RayTracer.Geometry;
import RayTracer.Vector3;

namespace RayTracer
{
    export template<GeometryConcept TGeometry>
    class IntersectionResult
    {
    public:
        const TGeometry* HitGeometry{nullptr};
        float Distance{0.0f};

        IntersectionResult(const TGeometry* hitGeometry, float distance)
            : HitGeometry{hitGeometry}, Distance{distance}
        {

        }

        inline operator IntersectionResult<Geometry>() const
        {
            return IntersectionResult<Geometry>{HitGeometry, Distance};
        }
    };
}