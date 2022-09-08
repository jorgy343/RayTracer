#include <cmath>
#include <concepts>

export module RayTracer.Vector3;

import RayTracer.Math;
import RayTracer.Vector2;

namespace RayTracer
{
    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    class alignas(sizeof(T) * 4) Vector3T
    {
    public:
        T X{0};
        T Y{0};
        T Z{0};

    private:
        T __DummyW{0};

    public:

        Vector3T() = default;

        explicit Vector3T(T scalar)
            : X{scalar}, Y{scalar}, Z{scalar}
        {

        }

        Vector3T(T x, T y, T z)
            : X{x}, Y{y}, Z{z}
        {

        }

        Vector3T(const Vector2T<T>& vector2, T z)
            : X{vector2.X}, Y{vector2.Y}, Z{z}
        {

        }

        inline Vector3T& Abs()
        {
            X = std::abs(X);
            Y = std::abs(Y);
            Z = std::abs(Z);

            return *this;
        }

        inline bool Compare(const Vector3T& right, T maximumAllowedErrorPerComponent)
        {
            if constexpr (std::floating_point<T>)
            {
                bool areNansBad =
                    std::isnan(X) ^ std::isnan(right.X) ||
                    std::isnan(Y) ^ std::isnan(right.Y) ||
                    std::isnan(Z) ^ std::isnan(right.Z);

                if (areNansBad)
                {
                    return false;
                }

                bool areInfinitiesBad =
                    std::isinf(X) ^ std::isinf(right.X) ||
                    std::isinf(Y) ^ std::isinf(right.Y) ||
                    std::isinf(Z) ^ std::isinf(right.Z);

                if (areInfinitiesBad)
                {
                    return false;
                }

                return
                    (!std::isfinite(X) || !std::isfinite(right.X) || std::abs(X - right.X) < maximumAllowedErrorPerComponent) &&
                    (!std::isfinite(Y) || !std::isfinite(right.Y) || std::abs(Y - right.Y) < maximumAllowedErrorPerComponent) &&
                    (!std::isfinite(Z) || !std::isfinite(right.Z) || std::abs(Z - right.Z) < maximumAllowedErrorPerComponent);
            }
            else
            {
                return
                    X == right.X &&
                    Y == right.Y &&
                    Z == right.Z;
            }
        }

        inline Vector3T ComponentwiseMultiply(const Vector3T& right) const
        {
            return Vector3T
            {
                X * right.X,
                Y * right.Y,
                Z * right.Z,
            };
        }

        inline Vector3T CrossProduct(const Vector3T& right) const
        {
            return Vector3T
            {
                Y * right.Z - Z * right.Y,
                Z * right.X - X * right.Z,
                X * right.Y - Y * right.X
            };
        }

        inline T Distance(const Vector3T& right)
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

        inline T DistanceSquared(const Vector3T& right)
        {
            T x = X - right.X;
            T y = Y - right.Y;
            T z = Z - right.Z;

            return x * x + y * y + z * z;
        }

        inline T Dot(const Vector3T& right) const
        {
            return (X * right.X) + (Y * right.Y) + (Z * right.Z);
        }

        inline T Length() const
        {
            if constexpr (std::derived_from<float, T>)
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
            return (X * X) + (Y * Y) + (Z * Z);
        }

        inline Vector3T& Normalize()
        {
            T inverseLength;
            if constexpr (std::same_as<float, T>)
            {
                inverseLength = FastReciprical(Length());
            }
            else
            {
                inverseLength = T{1} / Length();
            }

            X *= inverseLength;
            Y *= inverseLength;
            Z *= inverseLength;

            return *this;
        }

        inline Vector3T NormalizeNondestructive() const
        {
            Vector3T result = *this;
            return result.Normalize();
        }

        Vector3T operator+() const
        {
            return {+X, +Y, +Z};
        }

        Vector3T operator-() const
        {
            return {-X, -Y, -Z};
        }

        Vector3T& operator++()
        {
            X += T{1};
            Y += T{1};
            Z += T{1};

            return *this;
        }

        Vector3T& operator--()
        {
            X -= T{1};
            Y -= T{1};
            Z -= T{1};

            return *this;
        }

        Vector3T operator++(int)
        {
            Vector3T temp = *this;

            X += T{1};
            Y += T{1};
            Z += T{1};

            return temp;
        }

        Vector3T operator--(int)
        {
            Vector3T temp = *this;

            X -= T{1};
            Y -= T{1};
            Z -= T{1};

            return temp;
        }

        Vector3T operator+(const Vector3T& right) const
        {
            return Vector3T{X + right.X, Y + right.Y, Z + right.Z};
        }

        Vector3T operator-(const Vector3T& right) const
        {
            return Vector3T{X - right.X, Y - right.Y, Z - right.Z};
        }

        T operator*(const Vector3T& right) const
        {
            return Dot(right);
        }

        Vector3T operator%(const Vector3T& right) const
        {
            return CrossProduct(right);
        }

        Vector3T operator+(T right) const
        {
            return Vector3T{X + right, Y + right, Z + right};
        }

        Vector3T operator-(T right) const
        {
            return Vector3T{X - right, Y - right, Z - right};
        }

        Vector3T operator*(T right) const
        {
            return Vector3T{X * right, Y * right, Z * right};
        }

        Vector3T operator/(T right) const
        {
            return Vector3T{X / right, Y / right, Z / right};
        }

        Vector3T& operator+=(const Vector3T& other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;

            return *this;
        }

        Vector3T& operator-=(const Vector3T& other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;

            return *this;
        }

        Vector3T& operator+=(T other)
        {
            X += other;
            Y += other;
            Z += other;

            return *this;
        }

        Vector3T& operator-=(T other)
        {
            X -= other;
            Y -= other;
            Z -= other;

            return *this;
        }

        Vector3T& operator*=(T other)
        {
            X *= other;
            Y *= other;
            Z *= other;

            return *this;
        }

        Vector3T& operator/=(T other)
        {
            X /= other;
            Y /= other;
            Z /= other;

            return *this;
        }
    };

    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    Vector3T<T> operator+(T left, const Vector3T<T>& right)
    {
        return {left + right.X, left + right.Y, left + right.Z};
    }

    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    Vector3T<T> operator*(T left, const Vector3T<T>& right)
    {
        return {left * right.X, left * right.Y, left * right.Z};
    }

    export using Vector3 = Vector3T<float>;
    export using IntVector3 = Vector3T<int>;
    export using UIntVector3 = Vector3T<unsigned int>;
}