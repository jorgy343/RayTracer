#include <cmath>

export module RayTracer.Vector2;

import RayTracer.Math;

namespace RayTracer
{
    export class Vector2
    {
    public:
        float X{0.0f};
        float Y{0.0f};

        Vector2() = default;

        explicit Vector2(float scalar)
            : X{scalar}, Y{scalar}
        {

        }

        Vector2(float x, float y)
            : X{x}, Y{y}
        {

        }

        inline Vector2& Abs()
        {
            X = fabsf(X);
            Y = fabsf(Y);

            return *this;
        }

        inline bool Compare(const Vector2& right, float maximumAllowedErrorPerComponent)
        {
            bool areNansBad =
                std::isnan(X) ^ std::isnan(right.X) ||
                std::isnan(Y) ^ std::isnan(right.Y);

            if (areNansBad)
            {
                return false;
            }

            bool areInfinitiesBad =
                std::isinf(X) ^ std::isinf(right.X) ||
                std::isinf(Y) ^ std::isinf(right.Y);

            if (areInfinitiesBad)
            {
                return false;
            }

            return
                (!std::isfinite(X) || !std::isfinite(right.X) || fabsf(X - right.X) < maximumAllowedErrorPerComponent) &&
                (!std::isfinite(Y) || !std::isfinite(right.Y) || fabsf(Y - right.Y) < maximumAllowedErrorPerComponent);
        }

        inline Vector2 ComponentwiseMultiply(const Vector2& right) const
        {
            return Vector2
            {
                X * right.X,
                Y * right.Y,
            };
        }

        inline float Dot(const Vector2& right) const
        {
            return (X * right.X) + (Y * right.Y);
        }

        inline float Length() const
        {
            return FastSqrt(LengthSquared());
        }

        inline float LengthSquared() const
        {
            return (X * X) + (Y * Y);
        }

        inline Vector2& Normalize()
        {
            float inverseLength = FastReciprical(Length());

            X *= inverseLength;
            Y *= inverseLength;

            return *this;
        }

        Vector2 operator+() const
        {
            return {+X, +Y};
        }

        Vector2 operator-() const
        {
            return {-X, -Y};
        }

        Vector2& operator++()
        {
            X += 1.0f;
            Y += 1.0f;

            return *this;
        }

        Vector2& operator--()
        {
            X -= 1.0f;
            Y -= 1.0f;

            return *this;
        }

        Vector2 operator++(int)
        {
            Vector2 temp = *this;

            X += 1.0f;
            Y += 1.0f;

            return temp;
        }

        Vector2 operator--(int)
        {
            Vector2 temp = *this;

            X -= 1.0f;
            Y -= 1.0f;

            return temp;
        }

        Vector2 operator+(const Vector2& right) const
        {
            return Vector2{X + right.X, Y + right.Y};
        }

        Vector2 operator-(const Vector2& right) const
        {
            return Vector2{X - right.X, Y - right.Y};
        }

        float operator*(const Vector2& right) const
        {
            return Dot(right);
        }

        Vector2 operator+(float right) const
        {
            return Vector2{X + right, Y + right};
        }

        Vector2 operator-(float right) const
        {
            return Vector2{X - right, Y - right};
        }

        Vector2 operator*(float right) const
        {
            return Vector2{X * right, Y * right};
        }

        Vector2 operator/(float right) const
        {
            return Vector2{X / right, Y / right};
        }

        Vector2& operator+=(const Vector2& other)
        {
            X += other.X;
            Y += other.Y;

            return *this;
        }

        Vector2& operator-=(const Vector2& other)
        {
            X -= other.X;
            Y -= other.Y;

            return *this;
        }

        Vector2& operator+=(float other)
        {
            X += other;
            Y += other;

            return *this;
        }

        Vector2& operator-=(float other)
        {
            X -= other;
            Y -= other;

            return *this;
        }

        Vector2& operator*=(float other)
        {
            X *= other;
            Y *= other;

            return *this;
        }

        Vector2& operator/=(float other)
        {
            X /= other;
            Y /= other;

            return *this;
        }
    };

    export Vector2 operator+(float left, const Vector2& right)
    {
        return {left + right.X, left + right.Y};
    }

    export Vector2 operator*(float left, const Vector2& right)
    {
        return {left * right.X, left * right.Y};
    }
}