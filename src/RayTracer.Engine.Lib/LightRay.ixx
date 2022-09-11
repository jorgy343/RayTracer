export module RayTracer.LightRay;

import RayTracer.Vector3;
import RayTracer.Ray;

namespace RayTracer
{
    export class LightRay
    {
    public:
        Ray ShadowRay{};
        Vector3 PointOnLight{};

        inline constexpr LightRay() = default;

        inline constexpr LightRay(Ray shadowRay, Vector3 pointOnLight)
            : ShadowRay{shadowRay}, PointOnLight{pointOnLight}
        {

        }
    };
}