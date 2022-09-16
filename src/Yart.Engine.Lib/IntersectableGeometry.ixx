export module IntersectableGeometry;

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
}