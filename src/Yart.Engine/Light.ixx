export module Light;

import Vector3;

namespace Yart
{
    export class Scene;

    export class Light
    {
    public:
        Vector3 Color{};

        constexpr Light(const Vector3& color)
            : Color{color}
        {

        }

        virtual constexpr Vector3 GetDirectionToLight(const Vector3& hitPosition, const Vector3& hitNormal) const = 0;
        virtual bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& directionToLight) const = 0;
    };
}