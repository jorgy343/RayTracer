#include <limits>

export module RayTracer.Plane;

import RayTracer.LambertianMaterial;
import RayTracer.Math;
import RayTracer.Ray;
import RayTracer.Vector3;
import RayTracer.Geometry;
import RayTracer.RayResultType;

namespace RayTracer
{
    export class alignas(16) Plane final : public Geometry
    {
    public:
        Vector3 Normal{};
        float Distance{0.0f};
        const LambertianMaterial* Material{nullptr};

        Plane(const Vector3& normal, float distance, const LambertianMaterial* material)
            : Normal{normal}, Distance{distance}, Material{material}
        {

        }

        Plane(const Vector3& normal, const Vector3& point, const LambertianMaterial* material)
            : Normal{normal}, Distance{-(normal * point)}, Material{material}
        {

        }

        inline const LambertianMaterial* GetMaterial() const override final
        {
            return Material;
        }

        inline Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override final
        {
            return (ray.Direction * Normal) < 0.0f ? Normal : -Normal;
        }

        virtual float IntersectEntrance(const Ray& ray) const override final
        {
            return Intersect<RayResultType::Entrance>(ray);
        }

        virtual float IntersectExit(const Ray& ray) const override final
        {
            return Intersect<RayResultType::Exit>(ray);
        }

    private:
        template <RayResultType TRayResultType>
        inline float Intersect(const Ray& ray) const
        {
            float normalDotDirection = Normal * ray.Direction;
            float normalDotRayPosition = Normal * ray.Position;

            float entranceDistance = -(Distance + normalDotRayPosition) * FastReciprical(normalDotDirection);

            return entranceDistance < 0.0f ? std::numeric_limits<float>::infinity() : entranceDistance;
        }
    };
}