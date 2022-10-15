export module GgxMaterial;

import "Common.h";

import DiffuseMaterial;
import Geometry;
import Math;
import MonteCarlo;
import Random;
import Ray;
import Scene;

namespace Yart
{
	// Heavily based on: http://cwyman.org/code/dxrTutors/tutors/Tutor14/tutorial14.md.html
	export class GgxMaterial : public DiffuseMaterial
	{
	protected:
        Color3 SpecularColor{};
		real Roughness{};

		real DiffuseLuminance{};
		real SpecularLuminance{};

	public:
		GgxMaterial(const Color3& diffuseColor, const Color3& specularColor, real roughness)
			:
			DiffuseMaterial{diffuseColor},
			SpecularColor{specularColor},
			Roughness{roughness},
            DiffuseLuminance{Math::max(real{0.01}, diffuseColor.Luminance())},
            SpecularLuminance{Math::max(real{0.01}, specularColor.Luminance())}
		{

		}

		inline Color3 CalculateRenderingEquation(
            const Scene& scene,
            const Random& random,
            int currentDepth,
            const Geometry* hitGeometry,
            const Vector3& hitPosition,
            const Vector3& hitNormal,
            const Vector3& incomingDirection) const override
		{
			Vector3 V = -incomingDirection;
			real NdotV = hitNormal * V;







			real probDiffuse = ProbabilityToSampleDiffuse();
			bool chooseDiffuse = random.GetNormalized() < probDiffuse;

			if (chooseDiffuse)
			{
				// Shoot a randomly selected cosine-sampled diffuse ray.
				Vector3 L = GenerateCosineWeightedHemisphereSample(random, hitNormal);
                Color3 bounceColor = scene.CastRayColor({hitPosition, L}, currentDepth + 1, random);

				// Accumulate the color: (NdotL * incomingLight * dif / pi)
				// Probability of sampling this ray:  (NdotL / pi) * probDiffuse
                Color3 outputColor = bounceColor * DiffuseColor / probDiffuse;
				return outputColor;
			}
			else
			{
				Vector3 H = GetGgxMicrofacet(random, hitNormal);

				// Compute outgoing direction based on this (perfectly reflective) facet
                Vector3 L = (real{2.0} *(V * H) * H - V).Normalize();

				// Compute our color by tracing a ray in this direction
                Color3 bounceColor = scene.CastRayColor({hitPosition, L}, currentDepth + 1, random);

				// Compute some dot products needed for shading
				real NdotL = Math::max(real{0.0}, hitNormal * L);
				real NdotH = Math::max(real{0.0}, hitNormal * H);
				real LdotH = Math::max(real{0.0}, L * H);

				// Evaluate our BRDF using a microfacet BRDF model
				real D = NormalDistribution(NdotH);
				real G = SchlickMaskingTerm(NdotL, NdotV);
                Color3 F = SchlickFresnel(SpecularColor, LdotH);
                Color3 ggxTerm = D * G * F / (real{4.0} /** NdotL*/ *NdotV);

                real pdf = D * NdotH / (real{4.0} *LdotH);

                Color3 outputColor = bounceColor * ggxTerm /** NdotL*/ / (pdf * (real{1.0} - probDiffuse));
				return outputColor;
			}
		}

		inline constexpr real NormalDistribution(real nDotH) const
		{
			real a2 = Roughness * Roughness;
			real d = ((nDotH * a2 - nDotH) * nDotH + 1);

			return a2 / (d * d * Pi);
		}

		inline constexpr real SchlickMaskingTerm(real nDotL, real nDotV) const
		{
            real k = Roughness * Roughness / real{2.0};

			real g_v = nDotV / (nDotV * (real{1.0} - k) + k);
			real g_l = nDotL / (nDotL * (real{1.0} - k) + k);

			return g_v * g_l;
		}

		inline constexpr Color3 SchlickFresnel(const Color3& f0, real lDotH) const
		{
			real x = real{1.0} - lDotH;

			return f0 + (Color3{real{1.0}} - f0) * x * x * x * x * x;
		}

		inline Vector3 GetGgxMicrofacet(const Random& random, const Vector3& hitNormal) const
		{
			real rand1 = random.GetNormalized();
			real rand2 = random.GetNormalized();

			Vector3 B = Vector3::BuildPerpendicularVector(hitNormal);
			Vector3 T = B % hitNormal;

			real a2 = Roughness * Roughness;
			real cosThetaH = Math::sqrt(Math::max(real{0.0}, (real{1.0} - rand1) / ((a2 - real{1.0}) * rand1 + real{1.0})));
			real sinThetaH = Math::sqrt(Math::max(real{0.0}, real{1.0} - cosThetaH * cosThetaH));
            real phiH = rand2 * Pi * real{2.0};

			return T * (sinThetaH * Math::cos(phiH)) +
				B * (sinThetaH * Math::sin(phiH)) +
				hitNormal * cosThetaH;
		}

		inline constexpr real ProbabilityToSampleDiffuse() const
		{
			return DiffuseLuminance / (DiffuseLuminance + SpecularLuminance);
		}

		//inline constexpr real GetPdf(real D, real nDotH, real hDotV) const
		//{
		//    return D * nDotH / (4 * hDotV);
		//}
	};
}