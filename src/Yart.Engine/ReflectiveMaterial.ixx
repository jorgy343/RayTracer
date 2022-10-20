export module ReflectiveMaterial;

import "Common.h";

import Geometry;
import Material;
import Math;
import Random;
import Ray;
import Scene;

namespace Yart
{
    export class ReflectiveMaterial : public Material
    {
    public:
        inline Color3 CalculateRenderingEquation(
            const Scene& scene,
            const Random& random,
            int currentDepth,
            const Geometry* hitGeometry,
            const Vector3& hitPosition,
            const Vector3& hitNormal,
            const Vector3& incomingDirection,
            real mixAmount) const override
        {
            Vector3 reflectedDirection = incomingDirection.Reflect(hitNormal).Normalize();
            Ray outgoingRay = Ray{hitPosition, reflectedDirection};

            Color3 outputColor = scene.CastRayColor(outgoingRay, currentDepth + 1, random);
            return outputColor;
        }
    };
}