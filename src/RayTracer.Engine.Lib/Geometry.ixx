module;

export module Geometry;

import <concepts>;

import IntersectableGeometry;
import LambertianMaterial;
import Ray;
import Vector3;

namespace RayTracer
{
    export class Geometry : public IntersectableGeometry
    {
    public:
        virtual constexpr const LambertianMaterial* GetMaterial() const = 0;

        virtual constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition) const = 0;
    };

    export template<typename T>
    concept GeometryConcept = std::is_base_of<Geometry, T>::value;
}