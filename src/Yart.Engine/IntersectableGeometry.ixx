export module IntersectableGeometry;

import "Common.h";

import BoundingBox;
import IntersectionResult;
import Math;
import Ray;

namespace Yart
{
    export class __declspec(dllexport) IntersectableGeometry
    {
    public:
        virtual IntersectionResult IntersectEntrance(const Ray& ray) const = 0;
        virtual IntersectionResult IntersectExit(const Ray& ray) const = 0;

        virtual BoundingBox CalculateBoundingBox() const
        {
            return BoundingBox{
                Vector3{-std::numeric_limits<real>::infinity()},
                Vector3{std::numeric_limits<real>::infinity()},
            };
        }
    };

    export template<typename T>
        concept IntersectableGeometryConcept = std::is_base_of<IntersectableGeometry, T>::value;
}