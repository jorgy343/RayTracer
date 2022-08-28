export module RayTracer.DiffuseMaterial;

import RayTracer.Vector3;

namespace RayTracer
{
    export class DiffuseMaterial
    {
    public:
        Vector3 Color;
        Vector3 EmissiveColor;

        DiffuseMaterial(const Vector3& color, const Vector3& emissiveColor)
            : Color{color}, EmissiveColor{emissiveColor}
        {

        }
    };
}