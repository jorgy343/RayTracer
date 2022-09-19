export module EmissiveMaterial;

import Material;
import Ray;
import Scene;
import Vector3;

namespace Yart
{
    export class EmissiveMaterial : public Material
    {
	protected:
		Vector3 EmissiveColor{};

    public:
        EmissiveMaterial(const Vector3& emissiveColor)
            : EmissiveColor{emissiveColor}
        {

        }

        inline constexpr Vector3 CalculateRenderingEquation(const Scene& scene, int currentDepth, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const override
        {
            return EmissiveColor;
        }
    };
}