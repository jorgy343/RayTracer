export module DiffuseMaterial;

import Material;
import MonteCarlo;
import Random;
import Vector3;

namespace Yart
{
	export class DiffuseMaterial : public Material
	{
	protected:
		Vector3 DiffuseColor{};

		inline constexpr DiffuseMaterial(const Vector3& diffuseColor)
			: DiffuseColor{diffuseColor}
		{

		}

		inline constexpr Vector3 GenerateCosineWeightedHemisphereSample(const Random& random, const Vector3& hitNormal) const
		{
			float random1 = random.GetNormalizedFloat();
			float random2 = random.GetNormalizedFloat();

			Vector3 randomHemisphereVector = CosineWeightedSampleHemisphere(random1, random2);
			Vector3 outgoingDirection = TransformFromTangentSpaceToWorldSpace(hitNormal, randomHemisphereVector);

			return outgoingDirection;
		}
	};
}