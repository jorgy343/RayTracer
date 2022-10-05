export module DirectionalLight;

import "Common.h";

import Light;
import Math;
import Scene;

namespace Yart
{
    export class DirectionalLight : public Light
    {
    public:
        Vector3 Direction{};
        Vector3 ReversedDirection{};

        inline constexpr DirectionalLight(const Vector3& color, const Vector3& direction)
            : Light{color}, Direction{direction}, ReversedDirection{-direction}
        {

        }

        inline constexpr Vector3 GetDirectionTowardsLight(const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            return ReversedDirection;
        }

        inline bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& directionToLight) const override
        {
            Ray ray{hitPosition, Direction};
            float distance = scene.CastRayDistance(ray);

            return distance == std::numeric_limits<float>::infinity() ? false : true;
        }
    };
}