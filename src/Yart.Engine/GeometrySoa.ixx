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
        virtual void Insert(int index, std::shared_ptr<const TGeometry> geometry) = 0;
    };
}