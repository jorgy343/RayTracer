#include <cmath>

export module RayTracer.Vector3;

import RayTracer.Simd;
import RayTracer.Vector2;

namespace RayTracer
{
    export class Vector3
    {
    public:
        float X{0.0f};
        float Y{0.0f};
        float Z{0.0f};

        Vector3() = default;

        explicit Vector3(float scalar)
            : X{scalar}, Y{scalar}, Z{scalar}
        {

        }

        Vector3(float x, float y, float z)
            : X{x}, Y{y}, Z{z}
        {

        }

        Vector3(const Vector2& vector2, float z)
            : X{vector2.X}, Y{vector2.Y}, Z{z}
        {

        }

        inline Vector3& Abs()
        {
            X = fabsf(X);
            Y = fabsf(Y);
            Z = fabsf(Z);

            return *this;
        }

        inline Vector3 CrossProduct(const Vector3& right) const
        {
            return Vector3(
                Y * right.Z - Z * right.Y,
                Z * right.X - X * right.Z,
                X * right.Y - Y * right.X);
        }

        inline float Dot(const Vector3& right) const
        {
            return (X * right.X) + (Y * right.Y) + (Z * right.Z);
        }

        inline float Length() const
        {
            return UnsafeSqrt(LengthSquared());
        }

        inline float LengthSquared() const
        {
            return (X * X) + (Y * Y) + (Z * Z);
        }

        inline Vector3& Normalize()
        {
            float inverseLength = UnsafeReciprical(Length());

            X *= inverseLength;
            Y *= inverseLength;
            Z *= inverseLength;

            return *this;
        }

        Vector3 operator+() const
        {
            return {+X, +Y, +Z};
        }

        Vector3 operator-() const
        {
            return {-X, -Y, -Z};
        }

        Vector3& operator++()
        {
            X += 1.0f;
            Y += 1.0f;
            Z += 1.0f;

            return *this;
        }

        Vector3& operator--()
        {
            X -= 1.0f;
            Y -= 1.0f;
            Z -= 1.0f;

            return *this;
        }

        Vector3 operator++(int)
        {
            Vector3 temp = *this;

            X += 1.0f;
            Y += 1.0f;
            Z += 1.0f;

            return temp;
        }

        Vector3 operator--(int)
        {
            Vector3 temp = *this;

            X -= 1.0f;
            Y -= 1.0f;
            Z -= 1.0f;

            return temp;
        }

        Vector3 operator+(const Vector3& right) const
        {
            return Vector3{X + right.X, Y + right.Y, Z + right.Z};
        }

        Vector3 operator-(const Vector3& right) const
        {
            return Vector3{X - right.X, Y - right.Y, Z - right.Z};
        }

        float operator*(const Vector3& right) const
        {
            return Dot(right);
        }

        Vector3 operator%(const Vector3& right) const
        {
            return CrossProduct(right);
        }

        Vector3 operator+(float right) const
        {
            return Vector3{X + right, Y + right, Z + right};
        }

        Vector3 operator-(float right) const
        {
            return Vector3{X - right, Y - right, Z - right};
        }

        Vector3 operator*(float right) const
        {
            return Vector3{X * right, Y * right, Z * right};
        }

        Vector3 operator/(float right) const
        {
            return Vector3{X / right, Y / right, Z / right};
        }

        Vector3& operator+=(const Vector3& other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;

            return *this;
        }

        Vector3& operator-=(const Vector3& other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;

            return *this;
        }

        Vector3& operator+=(float other)
        {
            X += other;
            Y += other;
            Z += other;

            return *this;
        }

        Vector3& operator-=(float other)
        {
            X -= other;
            Y -= other;
            Z -= other;

            return *this;
        }

        Vector3& operator*=(float other)
        {
            X *= other;
            Y *= other;
            Z *= other;

            return *this;
        }

        Vector3& operator/=(float other)
        {
            X /= other;
            Y /= other;
            Z /= other;

            return *this;
        }
    };

    export Vector3 operator+(float left, const Vector3& right)
    {
        return {left + right.X, left + right.Y, left + right.Z};
    }

    export Vector3 operator*(float left, const Vector3& right)
    {
        return {left * right.X, left * right.Y, left * right.Z};
    }
}