export module RayTracer.Light;

import RayTracer.Vector3;
import RayTracer.Ray;
import RayTracer.LightRay;

namespace RayTracer
{
    export class Light
    {
    public:
        virtual LightRay CreateShadowRaw(const Vector3& hitPosition, const Vector3& hitNormal) const = 0;
        virtual Vector3 CalculateLightPower(const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& pointOnLight, float shadowDistance) const = 0;
    };
}