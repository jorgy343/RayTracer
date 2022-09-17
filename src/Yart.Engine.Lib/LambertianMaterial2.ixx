export module LambertianMaterial2;

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
		class LambertianMaterial2 : public DiffuseMaterial
	{
	public:
		inline constexpr LambertianMaterial2(const Vector3& diffuseColor)
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

			float whereToShootRay = scene.Rng.GetNormalizedFloat();

			if (whereToShootRay > 0.5f)
			{
				// Indirect light sample according to material.
				Vector3 outgoingDirection = GenerateCosineWeightedHemisphereSample(scene.Rng, hitNormal);
				Ray outgoingRay = Ray{hitPosition, outgoingDirection};

				Vector3 colorSample = scene.CastRayColor(outgoingRay, currentDepth + 1);
				Vector3 outputColor = DiffuseColor.ComponentwiseMultiply(colorSample) * roulettePower * 2.0f;

				return outputColor;
			}
			else
			{
				int indexOfLightToSample = scene.Rng.GetInteger() % scene.AreaLights.size();
				const AreaLight* light = scene.AreaLights[indexOfLightToSample];

				// Direct light sample to a random light.
				Vector3 outgoingDirection = light->GenerateRandomDirectionTowardsLight(scene.Rng, hitPosition, hitNormal);
				Ray outgoingRay = Ray{hitPosition, outgoingDirection};

				Vector3 colorSample = scene.CastRayColor(outgoingRay, currentDepth + 1);

				float brdf = OneOverPi;
				float inversePdf = light->CalculateInversePdf(scene.Rng, hitPosition, hitNormal, incomingDirection, outgoingDirection);
				float cosineTheta = Math::max(0.0f, hitNormal * outgoingDirection);

				Vector3 outputColor = brdf * DiffuseColor.ComponentwiseMultiply(colorSample) * inversePdf * cosineTheta * roulettePower * 2.0f;
				return outputColor;
			}
		}

		inline constexpr float CalculateInversePdf(const Vector3& hitNormal, const Vector3& outgoingDirection) const
		{
			float cosineTheta = Math::max(0.0f, hitNormal * outgoingDirection);
			float inverseCosineTheta = cosineTheta == 0.0f ? 0.0f : Math::rcp(cosineTheta);

			return Pi * inverseCosineTheta;
		}
	};
}