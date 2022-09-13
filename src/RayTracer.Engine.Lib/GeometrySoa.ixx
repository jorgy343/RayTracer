export module GeometrySoa;

import Ray;
import Geometry;
import IntersectableGeometry;
import IntersectionResult;

namespace RayTracer
{
    export template<GeometryConcept TGeometry>
    class GeometrySoa : public IntersectableGeometry
    {
    public:
        virtual constexpr void Insert(int index, const TGeometry* geometry) = 0;
    };
}