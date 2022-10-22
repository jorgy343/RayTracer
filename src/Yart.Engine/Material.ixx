export module Material;

import "Common.h";

import Math;
import Random;

namespace Yart
{
    export class Scene;
    export class Geometry;

    export class Material
    {
    public:
        virtual Color3 CalculateRenderingEquation(
            const Scene& scene,
            const Random& random,
            int currentDepth,
            const Geometry* hitGeometry,
            const Vector3& hitPosition,
            const Vector3& hitNormal,
            const Vector3& incomingDirection,
            real mixAmount) const = 0;
    };
}