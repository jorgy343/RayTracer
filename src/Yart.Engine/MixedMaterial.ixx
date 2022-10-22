export module MixedMaterial;

import "Common.h";

import Geometry;
import Material;
import Math;
import Random;
import Scene;

namespace Yart
{
    export class MixedMaterial : public Material
    {
    protected:
        const Material* LeftMaterial{};
        const Material* RightMaterial{};

    public:
        MixedMaterial(const Material* leftMaterial, const Material* rightMaterial)
            : LeftMaterial{leftMaterial}, RightMaterial{rightMaterial}
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
            if (mixAmount < Epsilon)
            {
                return LeftMaterial->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});
            }
            else if (real{1} - mixAmount < Epsilon)
            {
                return RightMaterial->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});
            }
            else
            {
                Color3 leftColor = LeftMaterial->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});
                Color3 rightColor = RightMaterial->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});

                return leftColor * (real{1} - mixAmount) + rightColor * mixAmount;
            }
        }
    };
}