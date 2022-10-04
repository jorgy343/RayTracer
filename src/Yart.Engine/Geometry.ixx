export module Geometry;

import <concepts>;

import GeometryForward;
import IntersectableGeometry;
import Material;
import Ray;
import Vector3;

namespace Yart
{
    export class __declspec(dllexport) Geometry : public IntersectableGeometry
    {
    public:
        virtual constexpr const Material* GetMaterial() const = 0;

        virtual constexpr Vector3 CalculateNormal(const Ray& ray, const Vector3& hitPosition, float additionalData) const = 0;
    };

    export template<typename T>
    concept GeometryConcept = std::is_base_of<Geometry, T>::value;
}