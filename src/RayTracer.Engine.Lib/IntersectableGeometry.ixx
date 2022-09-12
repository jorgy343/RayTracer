export module RayTracer.IntersectableGeometry;

import RayTracer.LambertianMaterial;
import RayTracer.Vector3;
import RayTracer.Ray;
import RayTracer.IntersectionResult;
import RayTracer.IntersectionResultType;

namespace RayTracer
{
    export class IntersectableGeometry
    {
    public:
        virtual constexpr IntersectionResult IntersectEntrance(const Ray& ray) const = 0;
        virtual constexpr IntersectionResult IntersectExit(const Ray& ray) const = 0;
    };
}