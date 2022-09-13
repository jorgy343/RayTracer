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
        virtual constexpr void Insert(int index, const TGeometry* geometry) = 0;
    };
}