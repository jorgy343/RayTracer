export module ConstantMixedMaterial;

import "Common.h";

import Geometry;
import Material;
import Math;
import Random;
import Scene;

namespace Yart
{
    export class ConstantMixedMaterial : public Material
    {
    protected:
        real LeftAmount{};
        real RightAmount{};

        const Material* LeftMaterial{};
        const Material* RightMaterial{};

    public:
        ConstantMixedMaterial(real leftAmount, real rightAmount, const Material* leftMaterial, const Material* rightMaterial)
            : LeftAmount{leftAmount}, RightAmount{rightAmount}, LeftMaterial{leftMaterial}, RightMaterial{rightMaterial}
        {

        }

        Color3 CalculateRenderingEquation(
            const Scene& scene,
            const Random& random,
            int currentDepth,
            const Geometry* hitGeometry,
            const Vector3& hitPosition,
            const Vector3& hitNormal,
            const Vector3& incomingDirection,
            real mixAmount) const
        {
            Color3 leftColor = LeftMaterial->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});
            Color3 rightColor = RightMaterial->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});

            // Ignore the passed in mix amount and use our own mix amount passed in via the constructor.
            return leftColor * LeftAmount + rightColor * RightAmount;
        }
    };
}