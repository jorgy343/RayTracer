export module LambertianMaterial;

import "Constants.h";

import Material;
import MonteCarlo;
import Random;
import Vector3;

namespace Yart
{
    export class LambertianMaterial : public Material
    {
    public:
        inline constexpr LambertianMaterial(const Vector3& emissiveColor, const Vector3& color)
            : Material{emissiveColor, color, false}
        {

        }

        constexpr Vector3 GenerateRandomDirection(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const override
        {
            float random1 = random.GetNormalizedFloat();
            float random2 = random.GetNormalizedFloat();

            Vector3 randomHemisphereVector = CosineWeightedSampleHemisphere(random1, random2);
            Vector3 outgoingDirection = TransformFromTangentSpaceToWorldSpace(hitNormal, randomHemisphereVector);

            return outgoingDirection;
        }

        inline constexpr float CalculateBrdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const override
        {
            return OneOverPi;
        }

        inline constexpr float CalculateInversePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const override
        {
            float cosineTheta = Math::max(0.0f, hitNormal * outgoingDirection);
            float inverseCosineTheta = cosineTheta == 0.0f ? 0.0f : Math::rcp(cosineTheta);

            return Pi * inverseCosineTheta;
        }
    };
}