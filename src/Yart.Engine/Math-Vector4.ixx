export module Math:Vector4;

import "Common.h";

import :Basics;
import :Color4Decl;
import :Vector2;
import :Vector3;
import :Vector4Decl;

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

        static inline constexpr Vector4T Abs(const Vector4T& value)
        {
            Vector4T result = value;
            return result.Abs();
        }

        static inline constexpr bool Compare(const Vector4T& left, const Vector4T& right, T maximumAllowedErrorPerComponent)
        {
            if constexpr (std::floating_point<T>)
            {
                bool areNansBad =
                    Math::isnan(left.X) ^ Math::isnan(right.X) ||
                    Math::isnan(left.Y) ^ Math::isnan(right.Y) ||
                    Math::isnan(left.Z) ^ Math::isnan(right.Z) ||
                    Math::isnan(left.W) ^ Math::isnan(right.W);

                if (areNansBad)
                {
                    return false;
                }

                bool areInfinitiesBad =
                    Math::isinf(left.X) ^ Math::isinf(right.X) ||
                    Math::isinf(left.Y) ^ Math::isinf(right.Y) ||
                    Math::isinf(left.Z) ^ Math::isinf(right.Z) ||
                    Math::isinf(left.W) ^ Math::isinf(right.W);

                if (areInfinitiesBad)
                {
                    return false;
                }

                return
                    (!Math::isfinite(left.X) || !Math::isfinite(right.X) || Math::abs(left.X - right.X) < maximumAllowedErrorPerComponent) &&
                    (!Math::isfinite(left.Y) || !Math::isfinite(right.Y) || Math::abs(left.Y - right.Y) < maximumAllowedErrorPerComponent) &&
                    (!Math::isfinite(left.Z) || !Math::isfinite(right.Z) || Math::abs(left.Z - right.Z) < maximumAllowedErrorPerComponent) &&
                    (!Math::isfinite(left.W) || !Math::isfinite(right.W) || Math::abs(left.W - right.W) < maximumAllowedErrorPerComponent);
            }
            else
            {
                return
                    left.X == right.X &&
                    left.Y == right.Y &&
                    left.Z == right.Z &&
                    left.W == right.W;
            }
        }

        static inline constexpr Vector4T ComponentwiseMultiply(const Vector4T& left, const Vector4T& right)
        {
            return Vector4T
            {
                left.X * right.X,
                left.Y * right.Y,
                left.Z * right.Z,
                left.W * right.W,
            };
        }

        static inline constexpr T Distance(const Vector4T& left, const Vector4T& right)
        {
            return Math::sqrt(DistanceSquared(left, right));
        }

        static inline constexpr T DistanceSquared(const Vector4T& left, const Vector4T& right)
        {
            T x = left.X - right.X;
            T y = left.Y - right.Y;
            T z = left.Z - right.Z;
            T w = left.W - right.W;

            return x * x + y * y + z * z + w * w;
        }

        static inline constexpr T Dot(const Vector4T& left, const Vector4T& right)
        {
            return (left.X * right.X) + (left.Y * right.Y) + (left.Z * right.Z) + (left.W * right.W);
        }

        inline constexpr Vector4T& Exp()
        {
            X = Math::exp(X);
            Y = Math::exp(Y);
            Z = Math::exp(Z);
            W = Math::exp(W);

            return *this;
        }

        static inline constexpr Vector4T Exp(const Vector4T& value)
        {
            Vector4T result = value;
            return result.Exp();
        }

        inline constexpr T Length() const
        {
            return Math::sqrt(LengthSquared());
        }

        inline constexpr T LengthSquared() const
        {
            return (X * X) + (Y * Y) + (Z * Z) + (W * W);
        }

        inline constexpr Vector4T& Log()
        {
            X = Math::log(X);
            Y = Math::log(Y);
            Z = Math::log(Z);
            W = Math::log(W);

            return *this;
        }

        static inline constexpr Vector4T Log(const Vector4T& value)
        {
            Vector4T result = value;
            return result.Log();
        }

        static inline constexpr Vector4T Max(const Vector4T& left, const Vector4T& right)
        {
            return Vector4T{
                Math::max(left.X, right.X),
                Math::max(left.Y, right.Y),
                Math::max(left.Z, right.Z),
                Math::max(left.W, right.W),
            };
        }

        static inline constexpr Vector4T Min(const Vector4T& left, const Vector4T& right)
        {
            return Vector4T{
                Math::min(left.X, right.X),
                Math::min(left.Y, right.Y),
                Math::min(left.Z, right.Z),
                Math::min(left.W, right.W),
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

        static inline constexpr Vector4T Normalize(const Vector4T& value)
        {
            Vector4T result = value;
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

        static inline constexpr Vector4T Reciprical(const Vector4T& value)
        {
            Vector4T result = value;
            return result.Reciprical();
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
            return Dot(*this, right);
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

        inline constexpr explicit operator Color4T<T>() const
        {
            return Color4T<T>{X, Y, Z, W};
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