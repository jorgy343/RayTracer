export module SignedDistanceCylinder;

import "Common.h";

import BoundingBox;
import Material;
import Math;
import SignedDistance;

namespace Yart
{
    export class SignedDistanceCylinder : public SignedDistance
    {
    public:
        Vector3 Start{};
        Vector3 End{};
        real Radius{};
        const Material* AppliedMaterial{nullptr};

        inline SignedDistanceCylinder(
            const Vector3& start,
            const Vector3& end,
            real radius,
            const Material* appliedMaterial)
            :
            Start{start},
            End{end},
            Radius{radius},
            AppliedMaterial{appliedMaterial}
        {

        }

        virtual BoundingBox CalculateBoundingBox() const override
        {
            // TODO: This will over estimate the AABB. Make it more precise.
            Vector3 min = Vector3::Min(Start, End);
            Vector3 max = Vector3::Max(Start, End);

            Vector3 radiusVec{Radius};

            return BoundingBox{
                min - radiusVec,
                max + radiusVec,
            };
        }

        virtual const Material* GetMaterial() const override
        {
            return AppliedMaterial;
        }

        virtual SignedDistanceResult ClosestDistance(const Vector3& point) const override
        {
            Vector3  ba = End - Start;
            Vector3  pa = point - Start;

            real baba = Vector3::Dot(ba, ba);
            real paba = Vector3::Dot(pa, ba);
            real x = (pa * baba - ba * paba).Length() - Radius * baba;
            real y = Math::abs(paba - baba * real{0.5}) - baba * real{0.5};
            real x2 = x * x;
            real y2 = y * y * baba;
            real d = (Math::max(x, y) < real{0.0}) ? -Math::min(x2, y2) : (((x > real{0.0}) ? x2 : real{0.0}) + ((y > real{0.0}) ? y2 : real{0.0}));

            return {Math::sign(d) * Math::sqrt(Math::abs(d)) / baba, real{0}};
        }
    };
}