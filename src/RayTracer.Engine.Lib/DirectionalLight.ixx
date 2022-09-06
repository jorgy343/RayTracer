export module RayTracer.DirectionalLight;

import RayTracer.Light;
import RayTracer.Vector3;

namespace RayTracer
{
    export class DirectionalLight : public Light
    {
    public:
        Vector3 Color{};
        Vector3 Direction{};
        Vector3 ReversedDirection{};

        DirectionalLight(const Vector3& color, const Vector3& direction)
            : Color{color}, Direction{direction}, ReversedDirection{-direction}
        {

        }
    };
}