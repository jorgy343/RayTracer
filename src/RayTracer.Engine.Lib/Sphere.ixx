#include <immintrin.h>
#include <limits>

export module RayTracer.Sphere;

import RayTracer.LambertianMaterial;
import RayTracer.Ray;
import RayTracer.Math;
import RayTracer.Vector3;
import RayTracer.Geometry;

namespace RayTracer
{
    export class alignas(16) Sphere final : public Geometry
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

        inline const LambertianMaterial* GetMaterial() const override final
        {
            return Material;
        }

        inline Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override final
        {
            return (hitPosition - Position).Normalize();
        }

        float Intersect(const Ray& ray) const override final
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

            return exitDistance >= 0.0f ? entranceDistance : std::numeric_limits<float>::infinity();
        }
    };
}