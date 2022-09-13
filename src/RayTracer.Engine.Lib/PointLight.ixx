export module PointLight;

import Light;
import LightRay;
import Ray;
import Vector3;

namespace RayTracer
{
    export class PointLight : public Light
    {
    public:
        Vector3 Color{};
        Vector3 Position{};

        inline constexpr PointLight() = default;

        inline constexpr PointLight(const Vector3& color, const Vector3& position)
            : Color{color}, Position{position}
        {

        }

        inline constexpr LightRay CreateShadowRaw(const Vector3& hitPosition, const Vector3& hitNormal) const override final
        {
            Vector3 directionToLight = (Position - hitPosition).Normalize();
            return LightRay{Ray{hitPosition, directionToLight}, Position};
        }

        inline constexpr Vector3 CalculateLightPower(const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& pointOnLight, float shadowDistance) const override final
        {
            Vector3 directionToLight = Position - hitPosition;
            float distanceToLightSquared = directionToLight.LengthSquared();

            directionToLight.Normalize();

            if (shadowDistance * shadowDistance >= distanceToLightSquared - 0.01f)
            {
                return Color * Math::max(0.0f, hitNormal * directionToLight);
            }

            return Vector3{};
        }
    };
}