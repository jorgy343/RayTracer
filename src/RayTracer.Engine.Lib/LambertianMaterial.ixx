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
        inline constexpr LambertianMaterial(const Vector3& emissiveColor, const Vector3& color)
            : Material{emissiveColor, color, false}
        {

        }

        constexpr ScatterResult CalculateScatterData(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const override
        {
            float random1 = random.GetNormalizedFloat();
            float random2 = random.GetNormalizedFloat();

            Vector3 randomHemisphereVector = CosineWeightedSampleHemisphere(random1, random2);
            Vector3 scatterDirection = TransformFromTangentSpaceToWorldSpace(hitNormal, randomHemisphereVector);

            float brdf = CalculateBrdf(random, hitPosition, hitNormal, scatterDirection);
            float pdf = CalculatePdf(random, hitPosition, hitNormal, scatterDirection);

            return {{hitPosition, scatterDirection}, brdf, pdf};
        }

        inline constexpr float CalculateBrdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const override
        {
            return OneOverPi;
        }

        inline constexpr float CalculatePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const override
        {
            return Math::max(0.0f, hitNormal * outgoingDirection) * OneOverPi;
        }
    };
}