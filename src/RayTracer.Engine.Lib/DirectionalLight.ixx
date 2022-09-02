export module RayTracer.DirectionalLight;

import RayTracer.Vector3;

namespace RayTracer
{
    export class DirectionalLight
    {
    public:
        Vector3 Color{};
        Vector3 Direction{};

        DirectionalLight(const Vector3& color, const Vector3& direction)
            : Color{color}, Direction{direction}
        {

        }
    };
}