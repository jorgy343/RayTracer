export module MirrorMaterial;

import "Constants.h";

import Material;
import MonteCarlo;
import Random;
import ScatterResult;
import Vector3;

namespace RayTracer
{
    export class MirrorMaterial final : public Material
    {
    public:
        inline constexpr MirrorMaterial()
            : Material{Vector3{0.0f}, Vector3{1.0f}, true}
        {

        }

        constexpr ScatterResult CalculateScatterData(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const override
        {
            Vector3 reflectedDirection = incomingDirection.Reflect(hitNormal).Normalize();

            return {{hitPosition, reflectedDirection}, 1.0f, 1.0f};
        }

        inline constexpr float CalculateBrdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const override
        {
            return 1.0f;
        }

        inline constexpr float CalculatePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const override
        {
            return 1.0f;
        }
    };
}