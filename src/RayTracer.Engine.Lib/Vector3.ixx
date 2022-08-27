#include <cmath>

export module RayTracer.Vector3;

namespace RayTracer
{
    export class Vector3
    {
    public:
        float X{};
        float Y{};
        float Z{};

        Vector3()
        {

        }

        Vector3(float scalar)
            : X{scalar}, Y{scalar}, Z{scalar}
        {

        }

        Vector3(float x, float y, float z)
            : X{x}, Y{y}, Z{z}
        {

        }

        Vector3& Normalize()
        {
            float inverseLength = 1.0f / Length();

            X *= inverseLength;
            Y *= inverseLength;
            Z *= inverseLength;

            return *this;
        }

        float Dot(const Vector3& right) const
        {
            return (X * right.X) + (Y * right.Y) + (Z * right.Z);
        }

        Vector3 CrossProduct(const Vector3& right) const
        {
            return Vector3(
                Y * right.Z - Z * right.Y,
                Z * right.X - X * right.Z,
                X * right.Y - Y * right.X);
        }

        float Length() const
        {
            return sqrtf(LengthSquared());
        }

        float LengthSquared() const
        {
            return (X * X) + (Y * Y) + (Z * Z);
        }

        Vector3& operator+()
        {
            return *this;
        }

        Vector3& operator-()
        {
            X = -X;
            Y = -Y;
            Z = -Z;

            return *this;
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
        return Vector3(left + right.X, left + right.Y, left + right.Z);
    }

    export Vector3 operator*(float left, const Vector3& right)
    {
        return Vector3(left * right.X, left * right.Y, left * right.Z);
    }
}