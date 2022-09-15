export module LambertianMaterial;

import "Constants.h";

import Material;
import MonteCarlo;
import Random;
import ScatterResult;
import Vector3;

namespace RayTracer
{
    export class LambertianMaterial final : public Material
    {
    public:
        Vector3 Color{};
        Vector3 EmissiveColor{};

        inline constexpr LambertianMaterial(const Vector3& color, const Vector3& emissiveColor)
            : Color{color}, EmissiveColor{emissiveColor}
        {

        }

        inline constexpr Vector3 CalculateIndirectLightDirection(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const
        {
            float random1 = random.GetNormalizedFloat();
            float random2 = random.GetNormalizedFloat();

            Vector3 randomHemisphereVector = CosineWeightedSampleHemisphere(random1, random2);
            Vector3 scatterDirection = TransformFromTangentSpaceToWorldSpace(hitNormal, randomHemisphereVector);

            return scatterDirection.Normalize();
        }

        inline constexpr float CalculatePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const
        {
            return Math::max(0.0f, hitNormal * outgoingDirection) * OneOverPi;
        }
    };
}