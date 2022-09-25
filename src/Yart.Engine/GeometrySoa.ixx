export module GeometrySoa;

import <memory>;

import Geometry;
import IntersectableGeometry;
import IntersectionResult;
import Ray;

namespace Yart
{
    export template<GeometryConcept TGeometry>
    class GeometrySoa : public IntersectableGeometry
    {
    public:
        virtual constexpr void Insert(size_t index, const TGeometry* geometry) = 0;
    };
}