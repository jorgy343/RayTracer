export module SignedDistanceRoundedAxisAlignedBox;

import "Common.h";

import BoundingBox;
import Material;
import Math;
import SignedDistance;

namespace Yart
{
    export class SignedDistanceRoundedAxisAlignedBox : public SignedDistance
    {
    public:
        Vector3 Minimum{};
        Vector3 Maximum{};
        real Radius{};
        const Material* AppliedMaterial{nullptr};

        inline SignedDistanceRoundedAxisAlignedBox(
            const Vector3& minimum,
            const Vector3& maximum,
            real radius,
            const Material* appliedMaterial)
            :
            Minimum{minimum},
            Maximum{maximum},
            Radius{radius},
            AppliedMaterial{appliedMaterial}
        {

        }

        virtual BoundingBox CalculateBoundingBox() const override
        {
            return BoundingBox{Minimum - Radius - Epsilon, Maximum + Radius + Epsilon};
        }

        virtual const Material* GetMaterial() const override
        {
            return AppliedMaterial;
        }

        virtual SignedDistanceResult ClosestDistance(const Vector3& point) const override
        {
            // Source: https://stackoverflow.com/a/30545544/1078268
            Vector3 distance = Vector3::Max(Minimum - point, point - Maximum);

            return
            {
                (Vector3::Max(distance, Vector3{real{0}})).Length() + Math::min(Math::max(distance.X, Math::max(distance.Y, distance.Z)), real{0}) - Radius,
                real{0},
            };
        }
    };
}