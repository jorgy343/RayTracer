export module SignedDistanceSmoothBinaryOperation;

import Math;
import MixedMaterial;
import SignedDistance;
import SignedDistanceBinaryOperator;

namespace Yart
{
    export template<SignedDistanceBinaryOperator Operator>
        class SignedDistanceSmoothBinaryOperation : public SignedDistance
    {
    protected:
        real SmoothingAmount{};

        const SignedDistance* Left{};
        const SignedDistance* Right{};

        const MixedMaterial* AppliedMaterial{};

    public:
        SignedDistanceSmoothBinaryOperation(real smoothingAmount, const SignedDistance* left, const SignedDistance* right, const MixedMaterial* appliedMaterial)
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

            if constexpr (Operator == SignedDistanceBinaryOperator::Union)
            {
                Vector2 result = SmoothMin(distanceLeft.Distance, distanceRight.Distance, SmoothingAmount);
                return {result.X, result.Y};
            }
            else if constexpr (Operator == SignedDistanceBinaryOperator::Intersection)
            {
                Vector2 result = SmoothMax(distanceLeft.Distance, distanceRight.Distance, SmoothingAmount);
                return {result.X, result.Y};
            }
            else
            {
                Vector2 result = SmoothMax(-distanceLeft.Distance, distanceRight.Distance, SmoothingAmount);
                return {result.X, result.Y};
            }
        }

    protected:
        // Source: https://iquilezles.org/articles/smin/
        Vector2 SmoothMin(real a, real b, real k) const
        {
            real h = Math::max(k - Math::abs(a - b), real{0}) / k;
            real m = h * h * real{0.5};
            real s = m * k * (real{1} / real{2});

            return (a < b) ? Vector2{a - s, real{1} - m} : Vector2{b - s, m};
        }

        Vector2 SmoothMax(real a, real b, real k) const
        {
            real h = Math::max(k - Math::abs(a - b), real{0}) / k;
            real m = h * h * real{0.5};
            real s = m * k * (real{1} / real{2});

            return (a > b) ? Vector2{a - s, m} : Vector2{b - s, real{1} - m};
        }
    };
}