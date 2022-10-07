export module DiffuseMaterial;

import "Common.h";

import Material;
import Math;
import MonteCarlo;
import Random;

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

		inline Vector3 GenerateCosineWeightedHemisphereSample(const Random& random, const Vector3& hitNormal) const
		{
			real random1 = random.GetNormalized();
			real random2 = random.GetNormalized();

			Vector3 randomHemisphereVector = CosineWeightedSampleHemisphere(random1, random2);
			Vector3 outgoingDirection = TransformFromTangentSpaceToWorldSpace(hitNormal, randomHemisphereVector);

			return outgoingDirection;
		}
	};
}