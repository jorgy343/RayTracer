export module RayTracer.DirectionalLight;

import RayTracer.Vector3;

namespace RayTracer
{
    export class DirectionalLight
    {
    public:
        Vector3 Color{};
        Vector3 Direction{};
    };
}