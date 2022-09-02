#include <immintrin.h>
#include <limits>

export module RayTracer.Sphere;

import RayTracer.LambertianMaterial;
import RayTracer.Ray;
import RayTracer.Math;
import RayTracer.Vector3;

namespace RayTracer
{
    export class alignas(16) Sphere
    {
    public:
        Vector3 Position{};
        float Radius{0.0f};
        const LambertianMaterial* Material{nullptr};

        Sphere() = default;

        Sphere(const Vector3& position, float radius, const LambertianMaterial* material)
            : Position{position}, Radius{radius}, Material{material}
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
            float discriminantSqrt = FastSqrt(discriminant);

            float reciprocalA = FastReciprical(a);
            float negativeB = -b;

            float exitDistance = (negativeB + discriminantSqrt) * reciprocalA;
            float entranceDistance = FastMax((negativeB - discriminantSqrt) * reciprocalA, 0.0f);

            float clampedEntranceDistance = exitDistance >= 0.0f ? entranceDistance : std::numeric_limits<float>::infinity();
            return clampedEntranceDistance;
        }
    };
}