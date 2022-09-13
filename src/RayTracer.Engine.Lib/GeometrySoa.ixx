export module GeometrySoa;

import Geometry;
import IntersectableGeometry;
import IntersectionResult;
import Ray;

namespace RayTracer
{
    export template<GeometryConcept TGeometry>
    class GeometrySoa : public IntersectableGeometry
    {
    public:
        virtual constexpr void Insert(int index, const TGeometry* geometry) = 0;
    };
}