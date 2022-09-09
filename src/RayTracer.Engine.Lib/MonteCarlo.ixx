#include <cmath>

#include "Constants.h"

export module RayTracer.MonteCarlo;

import RayTracer.Math;
import RayTracer.Vector3;

namespace RayTracer
{
    export inline Vector3 CosineWeightedSampleHemisphere(float random1, float random2)
    {
        float sinTheta = sqrtf(1 - random1 * random1);
        float phi = TwoPi * random2;
        float x = sinTheta * std::cosf(phi);
        float z = sinTheta * std::sinf(phi);

        return Vector3{x, random1, z};
    }

    export inline Vector3 TransformFromTangentSpaceToWorldSpace(const Vector3& normal, const Vector3& vectorToTransform)
    {
        Vector3 w = normal.NormalizeNondestructive();
        Vector3 a = std::fabsf(w.X > 0.9f) ? Vector3{0, 1, 0} : Vector3{1, 0, 0};
        Vector3 v = (w % a).Normalize();
        Vector3 u = w % v;

        return (vectorToTransform.X * u) + (vectorToTransform.Y * v) + (vectorToTransform.Z * w);
    }

    export inline Vector3 TransformTangentThing(const Vector3& hitNormal, const Vector3& vectorToTransform)
    {
        Vector3 nt;
        if (std::abs(hitNormal.X) > std::abs(hitNormal.Y))
        {
            nt = Vector3{hitNormal.Z, 0, -hitNormal.X} / FastSqrt((hitNormal.X * hitNormal.X) + (hitNormal.Z * hitNormal.Z));
        }
        else
        {
            nt = Vector3{0, -hitNormal.Z, hitNormal.Y} / FastSqrt((hitNormal.Y * hitNormal.Y) + (hitNormal.Z * hitNormal.Z));
        }

        Vector3 nb = hitNormal % nt;

        return {
            vectorToTransform.X * nb.X + vectorToTransform.Y * hitNormal.X + vectorToTransform.Z * nt.X,
            vectorToTransform.X * nb.Y + vectorToTransform.Y * hitNormal.Y + vectorToTransform.Z * nt.Y,
            vectorToTransform.X * nb.Z + vectorToTransform.Y * hitNormal.Z + vectorToTransform.Z * nt.Z,
        };
    }
}