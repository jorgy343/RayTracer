#include <limits>

export module RayTracer.Parallelogram;

import RayTracer.Math;
import RayTracer.Geometry;
import RayTracer.Vector3;
import RayTracer.IntersectionResult;
import RayTracer.IntersectionResultType;

namespace RayTracer
{
    export class alignas(16) Parallelogram : public Geometry
    {
    public:
        Vector3 Position{};
        Vector3 Edge1{};
        Vector3 Edge2{};
        Vector3 Normal{};
        float InverseArea{0.0f};
        const LambertianMaterial* Material{nullptr};

        inline constexpr Parallelogram() = default;

        inline constexpr Parallelogram(const Vector3& position, const Vector3& edge1, const Vector3& edge2, const LambertianMaterial* material)
            : Position{position}, Edge1{edge1}, Edge2{edge2}, Normal{(edge1 % edge2).Normalize()}, InverseArea{Math::rcp(edge1.Length() * edge2.Length())}, Material{material}
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

        constexpr IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            return {this, Intersect<IntersectionResultType::Entrance>(ray)};
        }

        constexpr IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            return {this, Intersect<IntersectionResultType::Exit>(ray)};
        }

        template <IntersectionResultType TIntersectionResultType>
        inline constexpr float Intersect(const Ray& ray) const
        {
            Vector3 p = ray.Direction % Edge2;
            float determinant = Edge1 * p;

            if (Math::abs(determinant) < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            float invDeterminant = Math::rcp(determinant);

            Vector3 t = ray.Position - Position;
            float a = (t * p) * invDeterminant;

            if (a < 0.0f || a > 1.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            Vector3 q = t % Edge1;
            float b = (ray.Direction * q) * invDeterminant;

            if (b < 0.0f || b > 1.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            float entranceDistance = (Edge2 * q) * invDeterminant;
            return entranceDistance >= 0.0f ? entranceDistance : std::numeric_limits<float>::infinity();
        }
    };
}