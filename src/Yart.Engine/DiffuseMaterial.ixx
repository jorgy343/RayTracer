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
        Color3 DiffuseColor{};

		DiffuseMaterial(const Color3& diffuseColor)
			: DiffuseColor{diffuseColor}
		{

		}

		Vector3 GenerateCosineWeightedHemisphereSample(const Random& random, const Vector3& hitNormal) const
		{
			real random1 = random.GetNormalized();
			real random2 = random.GetNormalized();

			Vector3 randomHemisphereVector = CosineWeightedSampleHemisphere(random1, random2);
			Vector3 outgoingDirection = TransformFromTangentSpaceToWorldSpace(hitNormal, randomHemisphereVector);

			return outgoingDirection;
		}
	};
}