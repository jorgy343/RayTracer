export module AreaLight;

import "Common.h";

import Geometry;
import Math;
import Random;

namespace Yart
{
    export class Scene;

    export class __declspec(dllexport) AreaLight : public Geometry
    {
    public:
        virtual Vector3 GetDirectionTowardsLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const = 0;
        virtual Vector3 GetPointOnLight(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal) const = 0;
        virtual bool IsInShadow(const Scene& scene, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& positionOnLight) const = 0;

        virtual constexpr real CalculateInversePdf(const Random& random, const Vector3& hitPosition, const Vector3& hitNormal, const Vector3& incomingDirection, const Vector3& outgoingDirection) const = 0;
    };
}