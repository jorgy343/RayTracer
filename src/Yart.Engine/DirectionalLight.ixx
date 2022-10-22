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

        DirectionalLight(const Color3& color, const Vector3& direction)
            : Light{color}, Direction{direction}, ReversedDirection{-direction}
        {

        }

        virtual Vector3 GetDirectionTowardsLight(const Vector3& hitPosition, const Vector3& hitNormal) const override
        {
            return ReversedDirection;
        }

        virtual bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& directionToLight) const override
        {
            Ray ray{hitPosition, Direction};
            real distance = scene.CastRayDistance(ray);

            return distance == std::numeric_limits<real>::infinity() ? false : true;
        }
    };
}