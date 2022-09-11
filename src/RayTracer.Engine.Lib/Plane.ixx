#include <limits>

export module RayTracer.Plane;

import RayTracer.LambertianMaterial;
import RayTracer.Math;
import RayTracer.Ray;
import RayTracer.Vector3;
import RayTracer.Geometry;
import RayTracer.IntersectionResultType;

namespace RayTracer
{
    export class alignas(16) Plane final : public Geometry
    {
    public:
        Vector3 Normal{};
        float Distance{0.0f};
        const LambertianMaterial* Material{nullptr};

        inline constexpr Plane() = default;

        inline constexpr Plane(const Vector3& normal, float distance, const LambertianMaterial* material)
            : Normal{normal}, Distance{distance}, Material{material}
        {

        }

        inline constexpr Plane(const Vector3& normal, const Vector3& point, const LambertianMaterial* material)
            : Normal{normal}, Distance{-(normal * point)}, Material{material}
        {

        }

        inline constexpr const LambertianMaterial* GetMaterial() const override final
        {
            return Material;
        }

        inline constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const override final
        {
            return (ray.Direction * Normal) < 0.0f ? Normal : -Normal;
        }

        constexpr float IntersectEntrance(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        constexpr float IntersectExit(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        inline constexpr float Intersect(const Ray& ray) const
        {
            float normalDotDirection = Normal * ray.Direction;
            float normalDotRayPosition = Normal * ray.Position;

            float entranceDistance = -(Distance + normalDotRayPosition) * Math::rcp(normalDotDirection);

            return entranceDistance >= 0.0f ? entranceDistance : std::numeric_limits<float>::infinity();
        }
    };
}