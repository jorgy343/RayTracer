#include <cmath>

export module RayTracer.Vector4;

import RayTracer.Math;
import RayTracer.Vector2;
import RayTracer.Vector3;

namespace RayTracer
{
    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    class alignas(sizeof(T) * 4) Vector4T
    {
    public:
        T X{0};
        T Y{0};
        T Z{0};
        T W{0};

        Vector4T() = default;

        explicit Vector4T(T scalar)
            : X{scalar}, Y{scalar}, Z{scalar}, W{scalar}
        {

        }

        Vector4T(T x, T y, T z, T w)
            : X{x}, Y{y}, Z{z}, W{w}
        {

        }

        Vector4T(const Vector2T<T>& vector2, T z, T w)
            : X{vector2.X}, Y{vector2.Y}, Z{z}, W{w}
        {

        }

        Vector4T(const Vector3T<T>& vector3, T w)
            : X{vector3.X}, Y{vector3.Y}, Z{vector3.Z}, W{w}
        {

        }

        inline Vector4T& Abs()
        {
            X = std::abs(X);
            Y = std::abs(Y);
            Z = std::abs(Z);
            W = std::abs(W);

            return *this;
        }

        inline bool Compare(const Vector4T& right, T maximumAllowedErrorPerComponent)
        {
            if constexpr (std::floating_point<T>)
            {
                bool areNansBad =
                    std::isnan(X) ^ std::isnan(right.X) ||
                    std::isnan(Y) ^ std::isnan(right.Y) ||
                    std::isnan(Z) ^ std::isnan(right.Z) ||
                    std::isnan(W) ^ std::isnan(right.W);

                if (areNansBad)
                {
                    return false;
                }

                bool areInfinitiesBad =
                    std::isinf(X) ^ std::isinf(right.X) ||
                    std::isinf(Y) ^ std::isinf(right.Y) ||
                    std::isinf(Z) ^ std::isinf(right.Z) ||
                    std::isinf(W) ^ std::isinf(right.W);

                if (areInfinitiesBad)
                {
                    return false;
                }

                return
                    (!std::isfinite(X) || !std::isfinite(right.X) || std::abs(X - right.X) < maximumAllowedErrorPerComponent) &&
                    (!std::isfinite(Y) || !std::isfinite(right.Y) || std::abs(Y - right.Y) < maximumAllowedErrorPerComponent) &&
                    (!std::isfinite(Z) || !std::isfinite(right.Z) || std::abs(Z - right.Z) < maximumAllowedErrorPerComponent) &&
                    (!std::isfinite(W) || !std::isfinite(right.W) || std::abs(W - right.W) < maximumAllowedErrorPerComponent);
            }
            else
            {
                return
                    X == right.X &&
                    Y == right.Y &&
                    Z == right.Z &&
                    W == right.W;
            }
        }

        inline Vector4T ComponentwiseMultiply(const Vector4T& right) const
        {
            return Vector4T
            {
                X * right.X,
                Y * right.Y,
                Z * right.Z,
                W * right.W,
            };
        }

        inline T Distance(const Vector4T& right)
        {
            if constexpr (std::same_as<float, T>)
            {
                return FastSqrt(DistanceSquared(right));
            }
            else
            {
                return std::sqrt(DistanceSquared(right));
            }
        }

        inline T DistanceSquared(const Vector4T& right)
        {
            T x = X - right.X;
            T y = Y - right.Y;
            T z = Z - right.Z;
            T w = W - right.W;

            return x * x + y * y + z * z + w * w;
        }

        inline T Dot(const Vector4T& right) const
        {
            return (X * right.X) + (Y * right.Y) + (Z * right.Z) + (W * right.W);
        }

        inline T Length() const
        {
            if constexpr (std::same_as<float, T>)
            {
                return FastSqrt(LengthSquared());
            }
            else
            {
                return std::sqrt(LengthSquared());
            }
        }

        inline T LengthSquared() const
        {
            return (X * X) + (Y * Y) + (Z * Z) + (W * W);
        }

        inline Vector4T& Normalize()
        {
            T inverseLength;
            if constexpr (std::same_as<float, T>)
            {
                inverseLength = FastRecipricalSqrt(LengthSquared());
            }
            else
            {
                inverseLength = T{1} / Length();
            }

            X *= inverseLength;
            Y *= inverseLength;
            Z *= inverseLength;
            W *= inverseLength;

            return *this;
        }

        Vector4T operator+() const
        {
            return {+X, +Y, +Z, +W};
        }

        Vector4T operator-() const
        {
            return {-X, -Y, -Z, -W};
        }

        Vector4T& operator++()
        {
            X += T{1};
            Y += T{1};
            Z += T{1};
            W += T{1};

            return *this;
        }

        Vector4T& operator--()
        {
            X -= T{1};
            Y -= T{1};
            Z -= T{1};
            W -= T{1};

            return *this;
        }

        Vector4T operator++(int)
        {
            Vector4T temp = *this;

            X += T{1};
            Y += T{1};
            Z += T{1};
            W += T{1};

            return temp;
        }

        Vector4T operator--(int)
        {
            Vector4T temp = *this;

            X -= T{1};
            Y -= T{1};
            Z -= T{1};
            W -= T{1};

            return temp;
        }

        Vector4T operator+(const Vector4T& right) const
        {
            return Vector4T{X + right.X, Y + right.Y, Z + right.Z, W + right.W};
        }

        Vector4T operator-(const Vector4T& right) const
        {
            return Vector4T{X - right.X, Y - right.Y, Z - right.Z, W - right.W};
        }

        T operator*(const Vector4T& right) const
        {
            return Dot(right);
        }

        Vector4T operator+(T right) const
        {
            return Vector4T{X + right, Y + right, Z + right, W + right};
        }

        Vector4T operator-(T right) const
        {
            return Vector4T{X - right, Y - right, Z - right, W - right};
        }

        Vector4T operator*(T right) const
        {
            return Vector4T{X * right, Y * right, Z * right, W * right};
        }

        Vector4T operator/(T right) const
        {
            return Vector4T{X / right, Y / right, Z / right, W / right};
        }

        Vector4T& operator+=(const Vector4T& other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;
            W += other.W;

            return *this;
        }

        Vector4T& operator-=(const Vector4T& other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;
            W -= other.W;

            return *this;
        }

        Vector4T& operator+=(T other)
        {
            X += other;
            Y += other;
            Z += other;
            W += other;

            return *this;
        }

        Vector4T& operator-=(T other)
        {
            X -= other;
            Y -= other;
            Z -= other;
            W -= other;

            return *this;
        }

        Vector4T& operator*=(T other)
        {
            X *= other;
            Y *= other;
            Z *= other;
            W *= other;

            return *this;
        }

        Vector4T& operator/=(T other)
        {
            X /= other;
            Y /= other;
            Z /= other;
            W /= other;

            return *this;
        }
    };

    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    Vector4T<T> operator+(T left, const Vector4T<T>& right)
    {
        return {left + right.X, left + right.Y, left + right.Z, left + right.W};
    }

    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    Vector4T<T> operator*(T left, const Vector4T<T>& right)
    {
        return {left * right.X, left * right.Y, left * right.Z, left * right.W};
    }

    export using Vector4 = Vector4T<float>;
    export using IntVector4 = Vector4T<int>;
    export using UIntVector4 = Vector4T<unsigned int>;
}