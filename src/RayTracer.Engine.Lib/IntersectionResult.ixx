export module RayTracer.IntersectionResult;

import RayTracer.Vector3;

namespace RayTracer
{
    export template<typename TGeometry>
        class IntersectionResult
    {
    public:
        const TGeometry* Geometry{nullptr};
        float Distance{0.0f};

        IntersectionResult(const TGeometry* geometry, float distance)
            : Geometry{geometry}, Distance{distance}
        {

        }
    };
}