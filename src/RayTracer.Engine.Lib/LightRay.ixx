export module LightRay;

import Ray;
import Vector3;

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