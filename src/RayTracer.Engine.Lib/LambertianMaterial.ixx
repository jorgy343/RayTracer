export module RayTracer.LambertianMaterial;

import RayTracer.Vector3;

namespace RayTracer
{
    export class LambertianMaterial
    {
    public:
        Vector3 Color;
        Vector3 EmissiveColor;

        LambertianMaterial(const Vector3& color, const Vector3& emissiveColor)
            : Color{color}, EmissiveColor{emissiveColor}
        {

        }
    };
}