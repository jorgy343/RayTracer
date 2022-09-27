export module MonteCarlo;

import <cmath>;
import <concepts>;

import "Common.h";

import Math;
import Vector3;

namespace Yart
{
    export template <std::floating_point T = real>
    inline constexpr Vector3 CosineWeightedSampleHemisphere(T random1, T random2)
    {
        // Source: https://www.scratchapixel.com/code.php?id=34&origin=/lessons/3d-basic-rendering/global-illumination-path-tracing

        T sinTheta = Math::sqrt(T{1} - random1 * random1);
        T phi = TwoPi * random2;

        T z = sinTheta * Math::sin(phi);
        T x = sinTheta * Math::cos(phi);

        return Vector3T<T>{x, random1, z};
    }

    export template <std::floating_point T = real>
    inline constexpr Vector3 TransformFromTangentSpaceToWorldSpace(const Vector3T<T>& hitNormal, const Vector3T<T>& vectorToTransform)
    {
        // Source: https://www.scratchapixel.com/code.php?id=34&origin=/lessons/3d-basic-rendering/global-illumination-path-tracing

        Vector3T<T> nt;
        if (Math::abs(hitNormal.X) > Math::abs(hitNormal.Y))
        {
            nt = Vector3T<T>{hitNormal.Z, T{0}, -hitNormal.X} / Math::sqrt((hitNormal.X * hitNormal.X) + (hitNormal.Z * hitNormal.Z));
        }
        else
        {
            nt = Vector3T<T>{T{0}, -hitNormal.Z, hitNormal.Y} / Math::sqrt((hitNormal.Y * hitNormal.Y) + (hitNormal.Z * hitNormal.Z));
        }

        Vector3T<T> nb = hitNormal % nt;

        return {
            (vectorToTransform.X * nb.X) + (vectorToTransform.Y * hitNormal.X) + (vectorToTransform.Z * nt.X),
            (vectorToTransform.X * nb.Y) + (vectorToTransform.Y * hitNormal.Y) + (vectorToTransform.Z * nt.Y),
            (vectorToTransform.X * nb.Z) + (vectorToTransform.Y * hitNormal.Z) + (vectorToTransform.Z * nt.Z),
        };
    }
}