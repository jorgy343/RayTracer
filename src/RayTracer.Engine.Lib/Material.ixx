export module RayTracer.Material;

import RayTracer.Ray;
import RayTracer.Vector3;
import RayTracer.ScatterResult;

namespace RayTracer
{
    export class Material
    {
        virtual ScatterResult GenerateScatterRay(const Ray& incomingRay, const Vector3& hitPosition, const Vector3& hitNormal) const = 0;
    };
}