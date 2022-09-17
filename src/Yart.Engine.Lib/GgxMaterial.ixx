export module GgxMaterial;

import "Constants.h";

import Material;
import Math;
import MonteCarlo;
import Random;
import Ray;
import Scene;
import Vector3;

namespace Yart
{
    // Heavily based on: http://cwyman.org/code/dxrTutors/tutors/Tutor14/tutorial14.md.html
    export class GgxMaterial : public Material
    {
    protected:
        Vector3 _diffuseColor{};
        Vector3 _specularColor{};
        float _roughness{};

        float _diffuseLuminance{};
        float _specularLuminance{};

    public:
        GgxMaterial(const Vector3& diffuseColor, const Vector3& specularColor, float roughness)
            :
            _diffuseColor{diffuseColor},
            _specularColor{specularColor},
            _roughness{roughness},
            _diffuseLuminance{Math::max(0.01f, diffuseColor * Vector3{0.3f, 0.59f, 0.11f})},
            _specularLuminance{Math::max(0.01f, specularColor * Vector3{0.3f, 0.59f, 0.11f})}
        {

        }

        inline constexpr Vector3 CalculateRenderingEquation(const Scene& scene, int currentDepth, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const override
        {
            Vector3 V = -incomingDirection;
            float NdotV = hitNormal * V;







            float probDiffuse = ProbabilityToSampleDiffuse();
            bool chooseDiffuse = scene.Rng.GetNormalizedFloat() < probDiffuse;
            
            if (chooseDiffuse)
            {
                // Shoot a randomly selected cosine-sampled diffuse ray.
                Vector3 L = GenerateRandomDiffuseDirection(scene.Rng, hitNormal);
                Vector3 bounceColor = scene.CastRayColor({hitPosition, L}, currentDepth + 1);

                // Accumulate the color: (NdotL * incomingLight * dif / pi)
                // Probability of sampling this ray:  (NdotL / pi) * probDiffuse
                Vector3 outputColor = bounceColor.ComponentwiseMultiply(_diffuseColor) / probDiffuse;
                return outputColor;
            }
            else
            {
                Vector3 H = GetGgxMicrofacet(scene.Rng, _roughness, hitNormal);

                // Compute outgoing direction based on this (perfectly reflective) facet
                Vector3 L = (2.0f * (V * H) * H - V).Normalize();

                // Compute our color by tracing a ray in this direction
                Vector3 bounceColor = scene.CastRayColor({hitPosition, L}, currentDepth + 1);

                // Compute some dot products needed for shading
                float NdotL = Math::max(0.0f, hitNormal * L);
                float NdotH = Math::max(0.0f, hitNormal * H);
                float LdotH = Math::max(0.0f, L * H);

                // Evaluate our BRDF using a microfacet BRDF model
                float D = NormalDistribution(NdotH, _roughness);
                float G = SchlickMaskingTerm(NdotL, NdotV, _roughness);
                Vector3 F = SchlickFresnel(_specularColor, LdotH);
                Vector3 ggxTerm = D * G * F / (4.0f /** NdotL*/ * NdotV);

                float pdf = D * NdotH / (4.0f * LdotH);

                Vector3 outputColor = bounceColor.ComponentwiseMultiply(ggxTerm) /** NdotL*/ / (pdf * (1.0f - probDiffuse));
                return outputColor;
            }
        }

        inline constexpr Vector3 GenerateRandomDiffuseDirection(const Random& random, const Vector3& hitNormal) const
        {
            float random1 = random.GetNormalizedFloat();
            float random2 = random.GetNormalizedFloat();

            Vector3 randomHemisphereVector = CosineWeightedSampleHemisphere(random1, random2);
            Vector3 outgoingDirection = TransformFromTangentSpaceToWorldSpace(hitNormal, randomHemisphereVector);

            return outgoingDirection;
        }

        inline constexpr float NormalDistribution(float nDotH, float roughness) const
        {
            float a2 = roughness * roughness;
            float d = ((nDotH * a2 - nDotH) * nDotH + 1);

            return a2 / (d * d * Pi);
        }

        inline constexpr float SchlickMaskingTerm(float nDotL, float nDotV, float roughness) const
        {
            float k = roughness * roughness / 2.0f;

            float g_v = nDotV / (nDotV * (1.0f - k) + k);
            float g_l = nDotL / (nDotL * (1.0f - k) + k);

            return g_v * g_l;
        }

        inline constexpr Vector3 SchlickFresnel(const Vector3& f0, float lDotH) const
        {
            float x = 1.0f - lDotH;

            return f0 + (Vector3{1.0f} - f0) * x * x * x * x * x;
        }

        inline constexpr Vector3 GetGgxMicrofacet(const Random& random, float roughness, const Vector3& hitNormal) const
        {
            float rand1 = random.GetNormalizedFloat();
            float rand2 = random.GetNormalizedFloat();

            Vector3 B = hitNormal.BuildPerpendicularVector();
            Vector3 T = B % hitNormal;

            float a2 = roughness * roughness;
            float cosThetaH = Math::sqrt(Math::max(0.0f, (1.0f - rand1) / ((a2 - 1.0f) * rand1 + 1.0f)));
            float sinThetaH = Math::sqrt(Math::max(0.0f, 1.0f - cosThetaH * cosThetaH));
            float phiH = rand2 * Pi * 2.0f;

            return T * (sinThetaH * Math::cos(phiH)) +
                B * (sinThetaH * Math::sin(phiH)) +
                hitNormal * cosThetaH;
        }

        inline constexpr float ProbabilityToSampleDiffuse() const
        {
            return _diffuseLuminance / (_diffuseLuminance + _specularLuminance);
        }

        //inline constexpr float GetPdf(float D, float nDotH, float hDotV) const
        //{
        //    return D * nDotH / (4 * hDotV);
        //}
    };
}