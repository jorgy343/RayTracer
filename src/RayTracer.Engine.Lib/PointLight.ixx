export module RayTracer.PointLight;

import RayTracer.Light;
import RayTracer.Vector3;

namespace RayTracer
{
    export class PointLight : public Light
    {
    public:
        Vector3 Color{};
        Vector3 Position{};

        inline constexpr PointLight(const Vector3& color, const Vector3& position)
            : Color{color}, Position{position}
        {

        }
    };
}