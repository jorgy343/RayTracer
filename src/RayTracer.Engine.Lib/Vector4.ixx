#include <cmath>

export module RayTracer.Vector4;

import RayTracer.Math;
import RayTracer.Vector2;
import RayTracer.Vector3;

namespace RayTracer
{
    export class alignas(16) Vector4
    {
    public:
        float X{0.0f};
        float Y{0.0f};
        float Z{0.0f};
        float W{0.0f};

        Vector4() = default;

        explicit Vector4(float scalar)
            : X{scalar}, Y{scalar}, Z{scalar}, W{scalar}
        {

        }

        Vector4(float x, float y, float z, float w)
            : X{x}, Y{y}, Z{z}, W{w}
        {

        }

        Vector4(const Vector2& vector2, float z, float w)
            : X{vector2.X}, Y{vector2.Y}, Z{z}, W{w}
        {

        }

        Vector4(const Vector3& vector3, float w)
            : X{vector3.X}, Y{vector3.Y}, Z{vector3.Z}, W{w}
        {

        }

        inline Vector4& Abs()
        {
            X = fabsf(X);
            Y = fabsf(Y);
            Z = fabsf(Z);
            W = fabsf(W);

            return *this;
        }

        inline float Dot(const Vector4& right) const
        {
            return (X * right.X) + (Y * right.Y) + (Z * right.Z) + (W * right.W);
        }

        inline float Length() const
        {
            return FastSqrt(LengthSquared());
        }

        inline float LengthSquared() const
        {
            return (X * X) + (Y * Y) + (Z * Z) + (W * W);
        }

        inline Vector4& Normalize()
        {
            float inverseLength = FastReciprical(Length());

            X *= inverseLength;
            Y *= inverseLength;
            Z *= inverseLength;
            W *= inverseLength;

            return *this;
        }

        Vector4 operator+() const
        {
            return {+X, +Y, +Z, +W};
        }

        Vector4 operator-() const
        {
            return {-X, -Y, -Z, -W};
        }

        Vector4& operator++()
        {
            X += 1.0f;
            Y += 1.0f;
            Z += 1.0f;
            W += 1.0f;

            return *this;
        }

        Vector4& operator--()
        {
            X -= 1.0f;
            Y -= 1.0f;
            Z -= 1.0f;
            W -= 1.0f;

            return *this;
        }

        Vector4 operator++(int)
        {
            Vector4 temp = *this;

            X += 1.0f;
            Y += 1.0f;
            Z += 1.0f;
            W += 1.0f;

            return temp;
        }

        Vector4 operator--(int)
        {
            Vector4 temp = *this;

            X -= 1.0f;
            Y -= 1.0f;
            Z -= 1.0f;
            W -= 1.0f;

            return temp;
        }

        Vector4 operator+(const Vector4& right) const
        {
            return Vector4{X + right.X, Y + right.Y, Z + right.Z, W + right.W};
        }

        Vector4 operator-(const Vector4& right) const
        {
            return Vector4{X - right.X, Y - right.Y, Z - right.Z, W - right.W};
        }

        float operator*(const Vector4& right) const
        {
            return Dot(right);
        }

        Vector4 operator+(float right) const
        {
            return Vector4{X + right, Y + right, Z + right, W + right};
        }

        Vector4 operator-(float right) const
        {
            return Vector4{X - right, Y - right, Z - right, W - right};
        }

        Vector4 operator*(float right) const
        {
            return Vector4{X * right, Y * right, Z * right, W * right};
        }

        Vector4 operator/(float right) const
        {
            return Vector4{X / right, Y / right, Z / right, W / right};
        }

        Vector4& operator+=(const Vector4& other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;
            W += other.W;

            return *this;
        }

        Vector4& operator-=(const Vector4& other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;
            W -= other.W;

            return *this;
        }

        Vector4& operator+=(float other)
        {
            X += other;
            Y += other;
            Z += other;
            W += other;

            return *this;
        }

        Vector4& operator-=(float other)
        {
            X -= other;
            Y -= other;
            Z -= other;
            W -= other;

            return *this;
        }

        Vector4& operator*=(float other)
        {
            X *= other;
            Y *= other;
            Z *= other;
            W *= other;

            return *this;
        }

        Vector4& operator/=(float other)
        {
            X /= other;
            Y /= other;
            Z /= other;
            W /= other;

            return *this;
        }
    };

    export Vector4 operator+(float left, const Vector4& right)
    {
        return {left + right.X, left + right.Y, left + right.Z, left + right.W};
    }

    export Vector4 operator*(float left, const Vector4& right)
    {
        return {left * right.X, left * right.Y, left * right.Z, left * right.W};
    }
}