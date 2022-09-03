#include <limits>

export module RayTracer.Plane;

import RayTracer.LambertianMaterial;
import RayTracer.Math;
import RayTracer.Ray;
import RayTracer.Vector3;

namespace RayTracer
{
    export class alignas(16) Plane
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

        inline Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const
        {
            return (ray.Direction * Normal) < 0.0f ? Normal : -Normal;
        }

        float Intersect(const Ray& ray) const
        {
            float normalDotDirection = Normal * ray.Direction;

            // Normally we would check if the dot product was normal, but let's just assume that never happens.

            float normalDotRayPosition = Normal * ray.Position;
            float entranceDistance = -(Distance + normalDotRayPosition) * FastReciprical(normalDotDirection);

            if (entranceDistance < 0.0f)
            {
                return std::numeric_limits<float>::infinity();
            }

            return entranceDistance;
        }
    };
}