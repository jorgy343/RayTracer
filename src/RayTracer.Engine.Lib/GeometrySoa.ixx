export module RayTracer.GeometrySoa;

import RayTracer.Ray;
import RayTracer.Geometry;
import RayTracer.IntersectableGeometry;
import RayTracer.IntersectionResult;

namespace RayTracer
{
    export template<GeometryConcept TGeometry>
    class GeometrySoa : public IntersectableGeometry
    {
    public:
        virtual constexpr long long GetCount() const = 0;
        virtual constexpr void Add(const TGeometry* geometry) = 0;
        virtual constexpr void Finalize() = 0;
    };
}