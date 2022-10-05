export module EmissiveMaterial;

import "Common.h";

import Geometry;
import Material;
import Math;
import Random;
import Ray;
import Scene;

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

        inline constexpr Vector3 CalculateRenderingEquation(
            const Scene& scene,
            const Random& random,
            int currentDepth,
            const Geometry* hitGeometry,
            const Vector3& hitPosition,
            const Vector3& hitNormal,
            const Vector3& incomingDirection) const override
        {
            return EmissiveColor;
        }
    };
}