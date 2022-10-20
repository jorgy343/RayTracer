export module SignedDistanceBinaryOperation;

import Math;
import MixedMaterial;
import SignedDistance;
import SignedDistanceBinaryOperator;

namespace Yart
{
    export template<SignedDistanceBinaryOperator Operator>
        class SignedDistanceBinaryOperation : public SignedDistance
    {
    protected:
        const SignedDistance* Left{};
        const SignedDistance* Right{};

        const MixedMaterial* AppliedMaterial{};

    public:
        SignedDistanceBinaryOperation(const SignedDistance* left, const SignedDistance* right, const MixedMaterial* appliedMaterial)
            : Left{left}, Right{right}, AppliedMaterial{appliedMaterial}
        {

        }

        constexpr BoundingBox CalculateBoundingBox() const override
        {
            return Left->CalculateBoundingBox().Union(Right->CalculateBoundingBox());
        }

        constexpr const Material* GetMaterial() const override
        {
            return AppliedMaterial;
        }

        SignedDistanceResult ClosestDistance(const Vector3& point) const override
        {
            SignedDistanceResult distanceLeft = Left->ClosestDistance(point);
            SignedDistanceResult distanceRight = Right->ClosestDistance(point);

            if constexpr (Operator == SignedDistanceBinaryOperator::Union)
            {
                return {Math::min(distanceLeft.Distance, distanceRight.Distance), distanceLeft.Distance > distanceRight.Distance ? real{0} : real{1}};
            }
            else if constexpr (Operator == SignedDistanceBinaryOperator::Intersection)
            {
                return {Math::max(distanceLeft.Distance, distanceRight.Distance), distanceLeft.Distance < distanceRight.Distance ? real{0} : real{1}};
            }
            else
            {
                return {Math::max(-distanceLeft.Distance, distanceRight.Distance), -distanceLeft.Distance < distanceRight.Distance ? real{0} : real{1}};
            }
        }
    };
}