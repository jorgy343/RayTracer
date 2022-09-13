export module LambertianMaterial;

import Material;
import Vector3;
import ScatterResult;

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