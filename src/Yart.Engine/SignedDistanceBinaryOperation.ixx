export module SignedDistanceBinaryOperation;

import Math;
import MixedMaterial;
import SignedDistance;
import SignedDistanceBinaryOperator;

namespace Yart
{
    export template<SignedDistanceBinaryOperator Operator, bool Smooth>
        class SignedDistanceBinaryOperation : public SignedDistance
    {
    protected:
        real SmoothingAmount{};

        const SignedDistance* Left{};
        const SignedDistance* Right{};

        const MixedMaterial* AppliedMaterial{};

    public:
        SignedDistanceBinaryOperation(real smoothingAmount, const SignedDistance* left, const SignedDistance* right, const MixedMaterial* appliedMaterial)
            : SmoothingAmount{smoothingAmount}, Left{left}, Right{right}, AppliedMaterial{appliedMaterial}
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

            if constexpr (!Smooth)
            {
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
                    return {Math::max(distanceLeft.Distance, -distanceRight.Distance), distanceLeft.Distance < -distanceRight.Distance ? real{0} : real{1}};
                }
            }
            else
            {
                if constexpr (Operator == SignedDistanceBinaryOperator::Union)
                {
                    Vector2 result = SmoothUnion(distanceLeft.Distance, distanceRight.Distance, SmoothingAmount);
                    return {result.X, result.Y};
                }
                else if constexpr (Operator == SignedDistanceBinaryOperator::Intersection)
                {
                    Vector2 result = SmoothIntersection(distanceLeft.Distance, distanceRight.Distance, SmoothingAmount);
                    return {result.X, result.Y};
                }
                else
                {
                    Vector2 result = SmoothDifference(distanceLeft.Distance, distanceRight.Distance, SmoothingAmount);
                    return {result.X, result.Y};
                }
            }
        }

    protected:
        // Source: https://iquilezles.org/articles/smin/
        Vector2 SmoothUnion(real a, real b, real k) const
        {
            real h = Math::max(k - Math::abs(a - b), real{0}) * Math::rcp(k);
            real m = h * h * real{0.5};
            real s = m * k * real{0.5};

            return
            {
                Math::min(a, b) - s,
                a < b ? real{1} - m : m,
            };
        }

        Vector2 SmoothIntersection(real a, real b, real k) const
        {
            real h = Math::max(k - Math::abs(a - b), real{0}) * Math::rcp(k);
            real m = h * h * real{0.5};
            real s = m * k * real{0.5};

            return
            {
                Math::max(a, b) + s,
                a > b ? real{1} - m : m,
            };
        }

        Vector2 SmoothDifference(real a, real b, real k) const
        {
            real h = Math::max(k - Math::abs(-a - b), real{0}) * Math::rcp(k);
            real m = h * h * real{0.5};
            real s = m * k * real{0.5};

            return
            {
                Math::max(a, -b) + s,
                a > -b ? real{1} - m : m,
            };
        }
    };
}