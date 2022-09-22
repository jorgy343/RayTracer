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

		constexpr Vector3 CalculateRenderingEquation(const Scene& scene, const Random& random, int currentDepth, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const override
		{
			float roulettePower{1.0f};
			if constexpr (EnableRoulette)
			{
				if (currentDepth > 3)
				{
					constexpr float stoppingCutoff = 0.7f;

					float stoppingProbability = random.GetNormalizedFloat();
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

			float whereToShootRay = random.GetNormalizedFloat();

			if (whereToShootRay > 0.5f)
			{
				// Indirect light sample according to material.
				Vector3 outgoingDirection = GenerateCosineWeightedHemisphereSample(random, hitNormal);
				Ray outgoingRay = Ray{hitPosition, outgoingDirection};

				Vector3 colorSample = scene.CastRayColor(outgoingRay, currentDepth + 1, random);
				Vector3 outputColor = DiffuseColor.ComponentwiseMultiply(colorSample) * roulettePower * 2.0f;

				return outputColor;
			}
			else
			{
				int indexOfLightToSample = random.GetInteger() % scene.AreaLights.size();
				auto& light = scene.AreaLights[indexOfLightToSample];

				// Direct light sample to a random light.
				Vector3 outgoingDirection = light->GenerateRandomDirectionTowardsLight(random, hitPosition, hitNormal);
				Ray outgoingRay = Ray{hitPosition, outgoingDirection};

				Vector3 colorSample = scene.CastRayColor(outgoingRay, currentDepth + 1, random);

				float brdf = OneOverPi;
				float inversePdf = light->CalculateInversePdf(random, hitPosition, hitNormal, incomingDirection, outgoingDirection);
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