export module LambertianMaterial;

import "Constants.h";

import AreaLight;
import DiffuseMaterial;
import Random;
import Ray;
import Scene;
import Vector3;

namespace Yart
{
    export template <bool EnableRoulette = false>
    class LambertianMaterial : public DiffuseMaterial
    {
    public:
        inline constexpr LambertianMaterial(const Vector3& diffuseColor)
            : DiffuseMaterial{diffuseColor}
        {

        }

        constexpr Vector3 CalculateRenderingEquation(const Scene& scene, int currentDepth, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const override
        {
            float roulettePower{1.0f};
            if constexpr (EnableRoulette)
            {
                if (currentDepth > 3)
                {
                    constexpr float stoppingCutoff = 0.7f;

                    float stoppingProbability = scene.Rng.GetNormalizedFloat();
                    if (stoppingProbability > stoppingCutoff)
                    {
                        return {};
                    }
                    else
                    {
                        roulettePower = Math::rcp(stoppingCutoff);
                    }
                }
            }

            int indexOfLightToSample = scene.Rng.GetInteger() % scene.AreaLights.size();
            const AreaLight* light = scene.AreaLights[indexOfLightToSample];

            float whereToShootRay = scene.Rng.GetNormalizedFloat();

            Vector3 outgoingDirection;
            if (whereToShootRay > 0.5f)
            {
                // Indirect light sample according to material.
                outgoingDirection = GenerateCosineWeightedHemisphereSample(scene.Rng, hitNormal);
            }
            else
            {
                // Direct light sample to a random light.
                outgoingDirection = light->GenerateRandomDirectionTowardsLight(scene.Rng, hitPosition, hitNormal);
            }

            float materialInversePdf = CalculateInversePdf(hitNormal, outgoingDirection);
            float lightInversePdf = light->CalculateInversePdf(scene.Rng, hitPosition, hitNormal, incomingDirection, outgoingDirection);
            float inversePdf = 0.5f * lightInversePdf * scene.AreaLights.size() + 0.5f * materialInversePdf;

            Ray outgoingRay = Ray{hitPosition, outgoingDirection};
            Vector3 indirectColorSample = scene.CastRayColor(outgoingRay, currentDepth + 1);

            float brdf = OneOverPi;
            float cosineTheta = Math::max(0.0f, hitNormal * outgoingDirection);

            Vector3 outputColor = brdf * DiffuseColor.ComponentwiseMultiply(indirectColorSample) * inversePdf * cosineTheta * roulettePower;
            return outputColor;
        }

        inline constexpr float CalculateInversePdf(const Vector3& hitNormal, const Vector3& outgoingDirection) const
        {
            float cosineTheta = Math::max(0.0f, hitNormal * outgoingDirection);
            float inverseCosineTheta = cosineTheta == 0.0f ? 0.0f : Math::rcp(cosineTheta);

            return Pi * inverseCosineTheta;
        }
    };
}