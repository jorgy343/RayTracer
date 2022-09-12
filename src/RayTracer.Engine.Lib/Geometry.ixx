#include <concepts>

export module RayTracer.Geometry;

import RayTracer.LambertianMaterial;
import RayTracer.Vector3;
import RayTracer.Ray;
import RayTracer.IntersectionResult;
import RayTracer.IntersectionResultType;

namespace RayTracer
{
    export class Geometry
    {
    public:
        virtual constexpr const LambertianMaterial* GetMaterial() const = 0;

        virtual constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const = 0;

        virtual constexpr IntersectionResult IntersectEntrance(const Ray& ray) const = 0;
        virtual constexpr IntersectionResult IntersectExit(const Ray& ray) const = 0;
    };

    export template<typename T>
    concept GeometryConcept = std::is_base_of<Geometry, T>::value;
}