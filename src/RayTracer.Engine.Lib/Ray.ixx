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

        inline constexpr Ray() = default;

        inline constexpr Ray(const Vector3& position, const Vector3& direction)
            : Position{position}, Direction{direction}, InverseDirection{Vector3{Math::rcp(direction.X), Math::rcp(direction.Y), Math::rcp(direction.Z)}}
        {

        }
    };
}