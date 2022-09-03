#include <concepts>

export module RayTracer.Geometry;

import RayTracer.LambertianMaterial;
import RayTracer.Vector3;
import RayTracer.Ray;

namespace RayTracer
{
    export class Geometry
    {
    public:
        virtual const LambertianMaterial* GetMaterial() const = 0;

        virtual Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const = 0;
        virtual float Intersect(const Ray& ray) const = 0;
    };

    export template<typename T>
    concept GeometryConcept = std::is_base_of<Geometry, T>::value;
}