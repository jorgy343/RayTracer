export module SignedDistance;

import "Common.h";

import BoundingBox;
import Geometry;
import Math;
import Material;

namespace Yart
{
    export class SignedDistance
    {
    public:
        const Material* AppliedMaterial{};

        SignedDistance(const Material* appliedMaterial)
            : AppliedMaterial{appliedMaterial}
        {

        }

        virtual constexpr BoundingBox CalculateBoundingBox() const
        {
            return BoundingBox{
                Vector3{-std::numeric_limits<real>::infinity()},
                Vector3{std::numeric_limits<real>::infinity()},
            };
        }

        virtual real ClosestDistance(const Vector3& point) const = 0;
    };
}