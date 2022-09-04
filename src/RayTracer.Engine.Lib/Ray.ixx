#include <cmath>
#include <limits>

export module RayTracer.Ray;

import RayTracer.Math;
import RayTracer.Vector3;

namespace RayTracer
{
    export class alignas(16) Ray
    {
    public:
        Vector3 Position{};
        Vector3 Direction{};
        Vector3 InverseDirection{};

        Ray() = default;

        Ray(const Vector3& position, const Vector3& direction)
            : Position{position}, Direction{direction}, InverseDirection{Vector3{FastReciprical(direction.X), FastReciprical(direction.Y), FastReciprical(direction.Z)}}
        {
            InverseDirection.X = std::isinf(InverseDirection.X) ? 99990000.0f : InverseDirection.X;
            InverseDirection.Y = std::isinf(InverseDirection.Y) ? 99990000.0f : InverseDirection.Y;
            InverseDirection.Z = std::isinf(InverseDirection.Z) ? 99990000.0f : InverseDirection.Z;
        }
    };
}