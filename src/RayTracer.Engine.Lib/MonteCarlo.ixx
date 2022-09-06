#include <cmath>

#include "Constants.h"

export module RayTracer.MonteCarlo;

import RayTracer.Math;
import RayTracer.Vector3;

namespace RayTracer
{
    export inline Vector3 CosineWeightedSampleHemisphere(float random1, float random2)
    {
        float r = FastSqrt(random1);
        float theta = TwoPi * random2;

        float x = r * std::cosf(theta);
        float y = r * std::sinf(theta);

        return Vector3{x, y, FastSqrt(FastMax(0.0f, 1.0f - random1))};
    }

    export inline Vector3 TransformFromTangentSpaceToWorldSpace(const Vector3& normal, const Vector3& vectorToTransform)
    {
        Vector3 w = normal.NormalizeNondestructive();
        Vector3 a = std::fabsf(w.X > 0.9f) ? Vector3{0, 1, 0} : Vector3{1, 0, 0};
        Vector3 v = (w % a).Normalize();
        Vector3 u = w % v;

        return (normal.X * u) + (normal.Y * v) + (normal.Z * w);
    }
}