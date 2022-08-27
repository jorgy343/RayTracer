export module RayTracer.Ray;

import RayTracer.Vector3;

namespace RayTracer
{
    export class alignas(16) Ray
    {
    public:
        Vector3 Position{};
        Vector3 Direction{};

        Ray() = default;

        Ray(const Vector3& position, const Vector3& direction)
            : Position{position}, Direction{direction}
        {

        }
    };
}