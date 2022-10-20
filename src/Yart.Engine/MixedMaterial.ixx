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
        const Material* Left{};
        const Material* Right{};

    public:
        MixedMaterial(const Material* left, const Material* right)
            : Left{left}, Right{right}
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
                return Left->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});
            }
            else if (real{1} - mixAmount < Epsilon)
            {
                return Right->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});
            }
            else
            {
                Color3 leftColor = Left->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});
                Color3 rightColor = Right->CalculateRenderingEquation(scene, random, currentDepth, hitGeometry, hitPosition, hitNormal, incomingDirection, real{0});

                return leftColor * (real{1} - mixAmount) + rightColor * mixAmount;
            }
        }
    };
}