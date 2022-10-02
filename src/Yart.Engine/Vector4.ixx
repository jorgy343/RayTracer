export module Vector4;

import <cmath>;

import "Common.h";

import Math;
import Vector2;
import Vector3;

namespace Yart
{
    export template <any_number T>
        class __declspec(dllexport) alignas(sizeof(T) * 4) Vector4T
    {
    public:
        T X{};
        T Y{};
        T Z{};
        T W{};

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

        inline constexpr T Distance(const Vector4T& right) const
        {
            return Math::sqrt(DistanceSquared(right));
        }

        inline constexpr T DistanceSquared(const Vector4T& right) const
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

        inline constexpr Vector4T Max(const Vector4T& other) const
        {
            return Vector4T{
                Math::max(X, other.X),
                Math::max(Y, other.Y),
                Math::max(Z, other.Z),
                Math::max(W, other.W),
            };
        }

        inline constexpr Vector4T Min(const Vector4T& other) const
        {
            return Vector4T{
                Math::min(X, other.X),
                Math::min(Y, other.Y),
                Math::min(Z, other.Z),
                Math::min(W, other.W),
            };
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

        inline constexpr Vector4T NormalizeConst() const
        {
            Vector4T result = *this;
            return result.Normalize();
        }

        inline constexpr Vector4T& Reciprical()
        {
            X = Math::rcp(X);
            Y = Math::rcp(Y);
            Z = Math::rcp(Z);
            W = Math::rcp(W);

            return *this;
        }

        inline constexpr Vector4T RecipricalConst() const
        {
            Vector4T result = *this;
            result.Reciprical();

            return result;
        }

        inline constexpr T& operator[](size_t index)
        {
            switch (index)
            {
                case 0: return X;
                case 1: return Y;
                case 2: return Z;
                case 3: return W;
                default: return X;
            }
        }

        inline constexpr const T& operator[](size_t index) const
        {
            switch (index)
            {
                case 0: return X;
                case 1: return Y;
                case 2: return Z;
                case 3: return W;
                default: return X;
            }
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

        template <any_number U>
        inline constexpr explicit operator Vector4T<U>() const
        {
            return Vector4T<U>(static_cast<U>(X), static_cast<U>(Y), static_cast<U>(Z), static_cast<U>(W));
        }
    };

    export template <any_number T>
        inline constexpr Vector4T<T> operator+(T left, const Vector4T<T>& right)
    {
        return {left + right.X, left + right.Y, left + right.Z, left + right.W};
    }

    export template <any_number T>
        inline constexpr Vector4T<T> operator*(T left, const Vector4T<T>& right)
    {
        return {left * right.X, left * right.Y, left * right.Z, left * right.W};
    }

    export using Vector4 = Vector4T<real>;
    export using FloatVector4 = Vector4T<float>;
    export using DoubleVector4 = Vector4T<double>;
    export using IntVector4 = Vector4T<int>;
    export using UIntVector4 = Vector4T<unsigned int>;
}