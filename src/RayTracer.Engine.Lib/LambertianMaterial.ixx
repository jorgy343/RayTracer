export module RayTracer.LambertianMaterial;

import RayTracer.Material;
import RayTracer.Vector3;
import RayTracer.ScatterResult;

namespace RayTracer
{
    export class LambertianMaterial final : public Material
    {
    public:
        Vector3 Color;
        Vector3 EmissiveColor;

        LambertianMaterial(const Vector3& color, const Vector3& emissiveColor)
            : Color{color}, EmissiveColor{emissiveColor}
        {

        }

        ScatterResult GenerateScatterRay(const Ray& incomingRay, const Vector3& hitPosition, const Vector3& hitNormal) const override final
        {
            return ScatterResult{Ray{}, 0.0f};
        }
    };
}