export module RayTracer.IntersectableGeometry;

import RayTracer.Ray;
import RayTracer.IntersectionResult;

namespace RayTracer
{
    export class IntersectableGeometry
    {
    public:
        virtual constexpr IntersectionResult IntersectEntrance(const Ray& ray) const = 0;
        virtual constexpr IntersectionResult IntersectExit(const Ray& ray) const = 0;
    };
}