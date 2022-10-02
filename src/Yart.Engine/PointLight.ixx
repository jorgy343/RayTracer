export module PointLight;

import Light;
import Math;
import Scene;
import Vector3;

namespace Yart
{
    export class PointLight : public Light
    {
    public:
        Vector3 Position{};

        inline constexpr PointLight(const Vector3& color, const Vector3& position)
            : Light{color}, Position{position}
        {

        }

        inline constexpr Vector3 GetDirectionToLight(const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            Vector3 directionToLight = (Position - hitPosition).Normalize();
            return directionToLight;
        }

        inline bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& directionToLight) const override
        {
            Vector3 directionToLight2 = Position - hitPosition;
            float distanceToLightSquared = directionToLight2.LengthSquared();

            directionToLight2.Normalize();

            Ray ray{hitPosition, directionToLight2};
            float distance = scene.CastRayDistance(ray);

            return distance * distance >= distanceToLightSquared - 0.01f ? false : true;
        }
    };
}