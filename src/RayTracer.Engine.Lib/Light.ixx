export module Light;

import Vector3;

namespace RayTracer
{
    export class Light
    {
    public:
        virtual Vector3 GenerateShadowDirection(const Vector3& hitPosition, const Vector3& hitNormal) const = 0;
        virtual Vector3 CalculateLightPower(const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& pointOnLight, float shadowDistance) const = 0;
    };
}