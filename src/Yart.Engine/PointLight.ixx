export module PointLight;

import "Common.h";

import Light;
import Math;
import Scene;

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

        inline constexpr Vector3 GetDirectionTowardsLight(const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            Vector3 directionToLight = (Position - hitPosition).Normalize();
            return directionToLight;
        }

        inline bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& directionToLight) const override
        {
            Vector3 directionToLight2 = Position - hitPosition;
            real distanceToLightSquared = directionToLight2.Length();

            directionToLight2.Normalize();

            Ray ray{hitPosition, directionToLight2};
            real distance = scene.CastRayDistance(ray);

            return distance >= distanceToLightSquared - real{0.01} ? false : true;
        }
    };
}