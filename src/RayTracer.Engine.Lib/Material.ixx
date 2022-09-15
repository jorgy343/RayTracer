export module Material;

import Random;
import ScatterResult;
import Vector3;

namespace RayTracer
{
    export class Material
    {
    public:
        Vector3 EmissiveColor{};
        Vector3 Color{};
        bool SkipLighting{false};

        inline constexpr Material(const Vector3& emissiveColor, const Vector3& color, bool skipLighting)
            : EmissiveColor{emissiveColor}, Color{color}, SkipLighting{skipLighting}
        {

        }

        virtual constexpr ScatterResult CalculateScatterData(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection) const = 0;

        virtual constexpr float CalculateBrdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const = 0;
        virtual constexpr float CalculatePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& outgoingDirection) const = 0;
    };
}