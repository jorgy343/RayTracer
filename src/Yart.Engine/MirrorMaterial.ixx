export module MirrorMaterial;

import "Constants.h";

import Material;
import Random;
import Ray;
import Scene;
import Vector3;

namespace Yart
{
    export class MirrorMaterial : public Material
    {
    public:
        inline constexpr MirrorMaterial()
        {

        }

        inline Vector3 CalculateRenderingEquation(const Scene& scene, const Random& random, int currentDepth, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const override
        {
            Vector3 reflectedDirection = incomingDirection.Reflect(hitNormal).Normalize();
            Ray outgoingRay = Ray{hitPosition, reflectedDirection};

            Vector3 outputColor = scene.CastRayColor(outgoingRay, currentDepth + 1, random);
            return outputColor;
        }
    };
}