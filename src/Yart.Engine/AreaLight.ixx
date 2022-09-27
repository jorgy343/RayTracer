export module AreaLight;

import Geometry;
import Random;
import Vector3;

namespace Yart
{
    export class __declspec(dllexport) AreaLight : public Geometry
    {
    public:
        virtual constexpr Vector3 GenerateRandomDirectionTowardsLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const = 0;
        virtual constexpr float CalculateInversePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection, const Vector3& outgoingDirection) const = 0;
    };
}