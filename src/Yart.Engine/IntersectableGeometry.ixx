export module IntersectableGeometry;

import <concepts>;

import IntersectionResult;
import Ray;

namespace Yart
{
    export class IntersectableGeometry
    {
    public:
        virtual constexpr IntersectionResult IntersectEntrance(const Ray& ray) const = 0;
        virtual constexpr IntersectionResult IntersectExit(const Ray& ray) const = 0;
    };

    export template<typename T>
        concept IntersectableGeometryConcept = std::is_base_of<IntersectableGeometry, T>::value;
}