export module MirrorMaterial;

import "Constants.h";

import Material;
import MonteCarlo;
import Random;
import Vector3;

namespace RayTracer
{
    export class MirrorMaterial final : public Material
    {
    public:
        inline constexpr MirrorMaterial()
            : Material{Vector3{0.0f}, Vector3{1.0f}}
        {

        }

        constexpr Vector3 GenerateRandomDirection(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const override
        {
            Vector3 reflectedDirection = incomingDirection.Reflect(hitNormal).Normalize();
            return reflectedDirection;
        }

        inline constexpr float CalculateBrdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const override
        {
            return 1.0f;
        }

        inline constexpr float CalculateInversePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const override
        {
            return 1.0f;
        }
    };
}