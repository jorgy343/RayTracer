module;

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

        inline constexpr Vector4T() = default;

        inline constexpr explicit Vector4T(T scalar)
            : X{scalar}, Y{scalar}, Z{scalar}, W{scalar}
        {

        }

        inline constexpr Vector4T(T x, T y, T z, T w)
            : X{x}, Y{y}, Z{z}, W{w}
        {

        }

        inline constexpr Vector4T(const Vector2T<T>& vector2, T z, T w)
            : X{vector2.X}, Y{vector2.Y}, Z{z}, W{w}
        {

        }

        inline constexpr Vector4T(const Vector3T<T>& vector3, T w)
            : X{vector3.X}, Y{vector3.Y}, Z{vector3.Z}, W{w}
        {

        }

        inline constexpr Vector4T& Abs()
        {
            X = Math::abs(X);
            Y = Math::abs(Y);
            Z = Math::abs(Z);
            W = Math::abs(W);

            return *this;
        }

        inline constexpr bool Compare(const Vector4T& right, T maximumAllowedErrorPerComponent)
        {
            if constexpr (std::floating_point<T>)
            {
                bool areNansBad =
                    Math::isnan(X) ^ Math::isnan(right.X) ||
                    Math::isnan(Y) ^ Math::isnan(right.Y) ||
                    Math::isnan(Z) ^ Math::isnan(right.Z) ||
                    Math::isnan(W) ^ Math::isnan(right.W);

                if (areNansBad)
                {
                    return false;
                }

                bool areInfinitiesBad =
                    Math::isinf(X) ^ Math::isinf(right.X) ||
                    Math::isinf(Y) ^ Math::isinf(right.Y) ||
                    Math::isinf(Z) ^ Math::isinf(right.Z) ||
                    Math::isinf(W) ^ Math::isinf(right.W);

                if (areInfinitiesBad)
                {
                    return false;
                }

                return
                    (!Math::isfinite(X) || !Math::isfinite(right.X) || Math::abs(X - right.X) < maximumAllowedErrorPerComponent) &&
                    (!Math::isfinite(Y) || !Math::isfinite(right.Y) || Math::abs(Y - right.Y) < maximumAllowedErrorPerComponent) &&
                    (!Math::isfinite(Z) || !Math::isfinite(right.Z) || Math::abs(Z - right.Z) < maximumAllowedErrorPerComponent) &&
                    (!Math::isfinite(W) || !Math::isfinite(right.W) || Math::abs(W - right.W) < maximumAllowedErrorPerComponent);
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

        inline constexpr Vector4T ComponentwiseMultiply(const Vector4T& right) const
        {
            return Vector4T
            {
                X * right.X,
                Y * right.Y,
                Z * right.Z,
                W * right.W,
            };
        }

        inline constexpr T Distance(const Vector4T& right)
        {
            return Math::sqrt(DistanceSquared(right));
        }

        inline constexpr T DistanceSquared(const Vector4T& right)
        {
            T x = X - right.X;
            T y = Y - right.Y;
            T z = Z - right.Z;
            T w = W - right.W;

            return x * x + y * y + z * z + w * w;
        }

        inline constexpr T Dot(const Vector4T& right) const
        {
            return (X * right.X) + (Y * right.Y) + (Z * right.Z) + (W * right.W);
        }

        inline constexpr T Length() const
        {
            return Math::sqrt(LengthSquared());
        }

        inline constexpr T LengthSquared() const
        {
            return (X * X) + (Y * Y) + (Z * Z) + (W * W);
        }

        inline constexpr Vector4T& Normalize()
        {
            T inverseLength;
            if constexpr (std::same_as<float, T>)
            {
                inverseLength = Math::inv_sqrt(LengthSquared());
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

        inline constexpr Vector4T operator+() const
        {
            return {+X, +Y, +Z, +W};
        }

        inline constexpr Vector4T operator-() const
        {
            return {-X, -Y, -Z, -W};
        }

        inline constexpr Vector4T& operator++()
        {
            X += T{1};
            Y += T{1};
            Z += T{1};
            W += T{1};

            return *this;
        }

        inline constexpr Vector4T& operator--()
        {
            X -= T{1};
            Y -= T{1};
            Z -= T{1};
            W -= T{1};

            return *this;
        }

        inline constexpr Vector4T operator++(int)
        {
            Vector4T temp = *this;

            X += T{1};
            Y += T{1};
            Z += T{1};
            W += T{1};

            return temp;
        }

        inline constexpr Vector4T operator--(int)
        {
            Vector4T temp = *this;

            X -= T{1};
            Y -= T{1};
            Z -= T{1};
            W -= T{1};

            return temp;
        }

        inline constexpr Vector4T operator+(const Vector4T& right) const
        {
            return Vector4T{X + right.X, Y + right.Y, Z + right.Z, W + right.W};
        }

        inline constexpr Vector4T operator-(const Vector4T& right) const
        {
            return Vector4T{X - right.X, Y - right.Y, Z - right.Z, W - right.W};
        }

        inline constexpr T operator*(const Vector4T& right) const
        {
            return Dot(right);
        }

        inline constexpr Vector4T operator+(T right) const
        {
            return Vector4T{X + right, Y + right, Z + right, W + right};
        }

        inline constexpr Vector4T operator-(T right) const
        {
            return Vector4T{X - right, Y - right, Z - right, W - right};
        }

        inline constexpr Vector4T operator*(T right) const
        {
            return Vector4T{X * right, Y * right, Z * right, W * right};
        }

        inline constexpr Vector4T operator/(T right) const
        {
            return Vector4T{X / right, Y / right, Z / right, W / right};
        }

        inline constexpr Vector4T& operator+=(const Vector4T& other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;
            W += other.W;

            return *this;
        }

        inline constexpr Vector4T& operator-=(const Vector4T& other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;
            W -= other.W;

            return *this;
        }

        inline constexpr Vector4T& operator+=(T other)
        {
            X += other;
            Y += other;
            Z += other;
            W += other;

            return *this;
        }

        inline constexpr Vector4T& operator-=(T other)
        {
            X -= other;
            Y -= other;
            Z -= other;
            W -= other;

            return *this;
        }

        inline constexpr Vector4T& operator*=(T other)
        {
            X *= other;
            Y *= other;
            Z *= other;
            W *= other;

            return *this;
        }

        inline constexpr Vector4T& operator/=(T other)
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
    inline constexpr Vector4T<T> operator+(T left, const Vector4T<T>&right)
    {
        return {left + right.X, left + right.Y, left + right.Z, left + right.W};
    }

    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    inline constexpr Vector4T<T> operator*(T left, const Vector4T<T>&right)
    {
        return {left * right.X, left * right.Y, left * right.Z, left * right.W};
    }

    export using Vector4 = Vector4T<float>;
    export using IntVector4 = Vector4T<int>;
    export using UIntVector4 = Vector4T<unsigned int>;

    static_assert(Vector4{0, -5, 0, 0}.Abs().Normalize().Length() == 1);
}