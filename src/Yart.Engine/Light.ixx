export module Light;

import "Common.h";

import Math;

namespace Yart
{
    export class Scene;

    export class Light
    {
    public:
        Color3 Color{};

        Light(const Color3& color)
            : Color{color}
        {

        }

        virtual Vector3 GetDirectionTowardsLight(const Vector3& hitPosition, const Vector3& hitNormal) const = 0;
        virtual bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& directionToLight) const = 0;
    };
}