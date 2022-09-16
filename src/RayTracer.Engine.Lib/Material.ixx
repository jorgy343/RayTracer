export module Material;

import Random;
import Vector3;

namespace RayTracer
{
    export class Material
    {
    public:
        Vector3 EmissiveColor{};
        Vector3 Color{};

        inline constexpr Material(const Vector3& emissiveColor, const Vector3& color)
            : EmissiveColor{emissiveColor}, Color{color}
        {

        }

        virtual constexpr Vector3 GenerateRandomDirection(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const = 0;

        virtual constexpr float CalculateBrdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const = 0;
        virtual constexpr float CalculateInversePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const = 0;
    };
}