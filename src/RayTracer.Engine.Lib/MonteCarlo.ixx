#include <cmath>

#include "Constants.h"

export module RayTracer.MonteCarlo;

import RayTracer.Math;
import RayTracer.Vector3;

namespace RayTracer
{
    export inline Vector3 CosineWeightedSampleHemisphere(float random1, float random2)
    {
        // Source: https://www.scratchapixel.com/code.php?id=34&origin=/lessons/3d-basic-rendering/global-illumination-path-tracing

        float sinTheta = Math::sqrt(1 - random1 * random1);
        float phi = TwoPi * random2;

        float z = sinTheta * std::sinf(phi);
        float x = sinTheta * std::cosf(phi);

        return Vector3{x, random1, z};
    }

    export inline constexpr Vector3 TransformFromTangentSpaceToWorldSpace(const Vector3& hitNormal, const Vector3& vectorToTransform)
    {
        // Source: https://www.scratchapixel.com/code.php?id=34&origin=/lessons/3d-basic-rendering/global-illumination-path-tracing

        Vector3 nt;
        if (Math::abs(hitNormal.X) > Math::abs(hitNormal.Y))
        {
            nt = Vector3{hitNormal.Z, 0, -hitNormal.X} / Math::sqrt((hitNormal.X * hitNormal.X) + (hitNormal.Z * hitNormal.Z));
        }
        else
        {
            nt = Vector3{0, -hitNormal.Z, hitNormal.Y} / Math::sqrt((hitNormal.Y * hitNormal.Y) + (hitNormal.Z * hitNormal.Z));
        }

        Vector3 nb = hitNormal % nt;

        return {
            (vectorToTransform.X * nb.X) + (vectorToTransform.Y * hitNormal.X) + (vectorToTransform.Z * nt.X),
            (vectorToTransform.X * nb.Y) + (vectorToTransform.Y * hitNormal.Y) + (vectorToTransform.Z * nt.Y),
            (vectorToTransform.X * nb.Z) + (vectorToTransform.Y * hitNormal.Z) + (vectorToTransform.Z * nt.Z),
        };
    }
}