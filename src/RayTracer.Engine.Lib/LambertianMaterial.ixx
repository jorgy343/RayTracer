export module RayTracer.LambertianMaterial;

import RayTracer.Material;
import RayTracer.Vector3;
import RayTracer.ScatterResult;

namespace RayTracer
{
    export class LambertianMaterial final : public Material
    {
    public:
        Vector3 Color{};
        Vector3 EmissiveColor{};

        inline constexpr LambertianMaterial(const Vector3& color, const Vector3& emissiveColor)
            : Color{color}, EmissiveColor{emissiveColor}
        {

        }
    };
}