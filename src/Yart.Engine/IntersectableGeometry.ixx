export module IntersectableGeometry;

import <concepts>;
import <limits>;

import Box;
import IntersectionResult;
import Ray;

namespace Yart
{
    export class IntersectableGeometry
    {
    public:
        virtual constexpr IntersectionResult IntersectEntrance(const Ray& ray) const = 0;
        virtual constexpr IntersectionResult IntersectExit(const Ray& ray) const = 0;

        virtual constexpr Box CalculateBoundingBox() const
        {
            return Box{
                Vector3{-std::numeric_limits<float>::infinity()},
                Vector3{std::numeric_limits<float>::infinity()},
            };
        }
    };

    export template<typename T>
        concept IntersectableGeometryConcept = std::is_base_of<IntersectableGeometry, T>::value;
}