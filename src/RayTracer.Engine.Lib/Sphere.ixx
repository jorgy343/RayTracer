#include <immintrin.h>
#include <limits>

export module RayTracer.Sphere;

import RayTracer.DiffuseMaterial;
import RayTracer.Ray;
import RayTracer.Simd;
import RayTracer.Vector3;

namespace RayTracer
{
    export class alignas(16) Sphere
    {
    public:
        Vector3 Position{};
        float Radius{0.0f};
        DiffuseMaterial* DiffuseMaterial{nullptr};

        Sphere() = default;

        Sphere(const Vector3& position, float radius)
            : Position{position}, Radius{radius}
        {

        }

        inline Vector3 CalculateNormal(const Vector3& hitPosition) const
        {
            return (hitPosition - Position).Normalize();
        }

        float Intersect(const Ray& ray) const
        {
            Vector3 v = ray.Position - Position;

            float a = ray.Direction * ray.Direction;
            float b = v * ray.Direction;
            float c = (v * v) - (Radius * Radius);

            float discriminant = (b * b) - (a * c);
            float discriminantSqrt = UnsafeSqrt(discriminant);

            float reciprocalA = UnsafeReciprical(a);
            float negativeB = -b;

            float exitDistance = (negativeB + discriminantSqrt) * reciprocalA;
            float entranceDistance = UnsafeMax((negativeB - discriminantSqrt) * reciprocalA, 0.0f);

            float clampedEntranceDistance = exitDistance >= 0.0f ? entranceDistance : std::numeric_limits<float>::infinity();
            return clampedEntranceDistance;
        }
    };
}