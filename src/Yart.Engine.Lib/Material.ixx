export module Material;

import Random;
import Vector3;

namespace Yart
{
    export class Scene;

    export class Material
    {
    public:
        virtual constexpr Vector3 CalculateRenderingEquation(const Scene& scene, int currentDepth, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const = 0;
    };
}