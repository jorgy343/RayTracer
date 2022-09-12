#include <limits>

export module RayTracer.DirectionalLight;

import RayTracer.Light;
import RayTracer.Vector3;
import RayTracer.Ray;
import RayTracer.Math;
import RayTracer.LightRay;

namespace RayTracer
{
    export class DirectionalLight final : public Light
    {
    public:
        Vector3 Color{};
        Vector3 Direction{};
        Vector3 ReversedDirection{};

        inline constexpr DirectionalLight() = default;

        inline constexpr DirectionalLight(const Vector3& color, const Vector3& direction)
            : Color{color}, Direction{direction}, ReversedDirection{-direction}
        {

        }

        inline constexpr LightRay CreateShadowRaw(const Vector3& hitPosition, const Vector3& hitNormal) const final override
        {
            return LightRay{Ray{hitPosition, ReversedDirection}, Vector3{}};
        }

        inline constexpr Vector3 CalculateLightPower(const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& pointOnLight, float shadowDistance) const final override
        {
            if (shadowDistance == std::numeric_limits<float>::infinity())
            {
                return Color * Math::max(0.0f, hitNormal * ReversedDirection);
            }

            return Vector3{};
        }
    };
}