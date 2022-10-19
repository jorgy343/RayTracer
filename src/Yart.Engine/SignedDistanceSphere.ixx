export module SignedDistanceSphere;

import "Common.h";

import Material;
import Math;
import SignedDistance;

namespace Yart
{
    export class SignedDistanceSphere : public SignedDistance
    {
    public:
        Vector3 Position{};
        real Radius{};

        SignedDistanceSphere(const Vector3& position, real radius, const Material* appliedMaterial)
            : SignedDistance{appliedMaterial}, Position{position}, Radius{radius}
        {

        }

        virtual real ClosestDistance(const Vector3& point) const override
        {
            return Vector3::Distance(point, Position) - Radius;
        }
    };
}