export module LambertianMaterial;

import "Common.h";

import AreaLight;
import DiffuseMaterial;
import Geometry;
import Math;
import Random;
import Ray;
import Scene;

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

		constexpr Vector3 CalculateRenderingEquation(
            const Scene& scene,
            const Random& random,
            int currentDepth,
            const Geometry* hitGeometry,
            const Vector3& hitPosition,
            const Vector3& hitNormal,
            const Vector3& incomingDirection) const override
		{
			real roulettePower{real{1.0}};
			if constexpr (EnableRoulette)
			{
				if (currentDepth > 3)
				{
                    constexpr real stoppingCutoff = real{0.7};

					real stoppingProbability = random.GetNormalized();
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

			real whereToShootRay = random.GetNormalized();

            if (whereToShootRay > real{0.5})
			{
				// Indirect light sample according to material.
				Vector3 outgoingDirection = GenerateCosineWeightedHemisphereSample(random, hitNormal);
				Ray outgoingRay = Ray{hitPosition, outgoingDirection};

				Vector3 colorSample = scene.CastRayColor(outgoingRay, currentDepth + 1, random);
				Vector3 outputColor = DiffuseColor.ComponentwiseMultiply(colorSample) * roulettePower * real{2.0};

				return outputColor;
			}
			else
			{
				int indexOfLightToSample = random.GetInteger(0, static_cast<int>(scene.AreaLights.size()) - 1);
				auto& light = scene.AreaLights[indexOfLightToSample];

				// Direct light sample to a random light.
				Vector3 outgoingDirection = light->GetDirectionTowardsLight(random, hitPosition, hitNormal);
				Ray outgoingRay = Ray{hitPosition, outgoingDirection};

				Vector3 colorSample = scene.CastRayColor(outgoingRay, currentDepth + 1, random);

				real brdf = OneOverPi;
				real inversePdf = light->CalculateInversePdf(random, hitPosition, hitNormal, incomingDirection, outgoingDirection);
				real cosineTheta = Math::max(real{0.0}, hitNormal * outgoingDirection);

				Vector3 outputColor = brdf * DiffuseColor.ComponentwiseMultiply(colorSample) * inversePdf * cosineTheta * roulettePower * real{2.0};
				return outputColor;
			}
		}

		inline constexpr real CalculateInversePdf(const Vector3& hitNormal, const Vector3& outgoingDirection) const
		{
			real cosineTheta = Math::max(real{0.0}, hitNormal * outgoingDirection);
			real inverseCosineTheta = cosineTheta == real{0.0} ? real{0.0} : Math::rcp(cosineTheta);

			return Pi * inverseCosineTheta;
		}
	};
}