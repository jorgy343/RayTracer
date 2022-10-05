export module Math:Vector3;

import "Common.h";

import :Basics;
import :Vector2;

namespace Yart
{
    export template <any_number T>
        class __declspec(dllexport) alignas(sizeof(T) * 4) Vector3T
    {
    public:
        T X{};
        T Y{};
        T Z{};

    private:
        T __DummyW{0};

    public:
        inline constexpr Vector3T() = default;

        inline constexpr explicit Vector3T(T scalar)
            : X{scalar}, Y{scalar}, Z{scalar}
        {

        }

        inline constexpr Vector3T(T x, T y, T z)
            : X{x}, Y{y}, Z{z}
        {

        }

        inline constexpr Vector3T(const Vector2T<T>& vector2, T z)
            : X{vector2.X}, Y{vector2.Y}, Z{z}
        {

        }

        inline constexpr Vector3T& Abs()
        {
            X = Math::abs(X);
            Y = Math::abs(Y);
            Z = Math::abs(Z);

            return *this;
        }

        inline constexpr Vector3T AbsConst() const
        {
            Vector3T result = *this;
            result.Abs();

            return result;
        }

        inline constexpr Vector3T BuildPerpendicularVector() const
        {
            // From: Efficient Construction of Perpendicular Vectors Without Branching.
            Vector3T a = this->AbsConst();

            unsigned int xm = ((a.X - a.Y) < 0 && (a.X - a.Z) < 0) ? 1 : 0;
            unsigned int ym = (a.Y - a.Z) < 0 ? (1 ^ xm) : 0;
            unsigned int zm = 1 ^ (xm | ym);

            return *this % Vector3T(static_cast<T>(xm), static_cast<T>(ym), static_cast<T>(zm));
        }

        inline constexpr bool Compare(const Vector3T& right, T maximumAllowedErrorPerComponent)
        {
            if constexpr (std::floating_point<T>)
            {
                bool areNansBad =
                    Math::isnan(X) ^ Math::isnan(right.X) ||
                    Math::isnan(Y) ^ Math::isnan(right.Y) ||
                    Math::isnan(Z) ^ Math::isnan(right.Z);

                if (areNansBad)
                {
                    return false;
                }

                bool areInfinitiesBad =
                    Math::isinf(X) ^ Math::isinf(right.X) ||
                    Math::isinf(Y) ^ Math::isinf(right.Y) ||
                    Math::isinf(Z) ^ Math::isinf(right.Z);

                if (areInfinitiesBad)
                {
                    return false;
                }

                return
                    (!Math::isfinite(X) || !Math::isfinite(right.X) || Math::abs(X - right.X) < maximumAllowedErrorPerComponent) &&
                    (!Math::isfinite(Y) || !Math::isfinite(right.Y) || Math::abs(Y - right.Y) < maximumAllowedErrorPerComponent) &&
                    (!Math::isfinite(Z) || !Math::isfinite(right.Z) || Math::abs(Z - right.Z) < maximumAllowedErrorPerComponent);
            }
            else
            {
                return
                    X == right.X &&
                    Y == right.Y &&
                    Z == right.Z;
            }
        }

        inline constexpr Vector3T ComponentwiseMultiply(const Vector3T& right) const
        {
            return Vector3T
            {
                X * right.X,
                Y * right.Y,
                Z * right.Z,
            };
        }

        inline constexpr Vector3T CrossProduct(const Vector3T& right) const
        {
            return Vector3T
            {
                Y * right.Z - Z * right.Y,
                Z * right.X - X * right.Z,
                X * right.Y - Y * right.X
            };
        }

        inline constexpr T Distance(const Vector3T& right) const
        {
            return Math::sqrt(DistanceSquared(right));
        }

        inline constexpr T DistanceSquared(const Vector3T& right) const
        {
            T x = X - right.X;
            T y = Y - right.Y;
            T z = Z - right.Z;

            return x * x + y * y + z * z;
        }

        inline constexpr T Dot(const Vector3T& right) const
        {
            return (X * right.X) + (Y * right.Y) + (Z * right.Z);
        }

        //static constexpr void Fresnel(const Vector3T& direction, const Vector3T& normal, T fromIndex, T toIndex, Vector3T* reflection, Vector3T* refraction, T* reflectionCoefficient, T* refractionCoefficient)
        //{
        //    Vector3T b = Dot(direction, normal);
        //    Vector3T c = -b;

        //    T n = fromIndex / toIndex;
        //    T sinT2 = n * n * (T{1} - c * c);

        //    // Reflection
        //    *reflection = direction - T{2} * b * normal;

        //    // Refraction
        //    T refractionRadicand = T{1} - sinT2;
        //    if (refractionRadicand < T{0})
        //    {
        //        *refraction = Vector3T(T{0});
        //    }

        //    *refraction = (n * direction) + (n * c - Math::sqrt(refractionRadicand)) * normal;

        //    // Reflection Coefficient
        //    T r0 = (fromIndex - toIndex) / (fromIndex + toIndex);
        //    r0 *= r0;

        //    T cosX = c;
        //    if (fromIndex > toIndex)
        //    {
        //        cosX = Math::sqrt(T{1} - cosX);
        //    }

        //    T x = T{1} - cosX;
        //    *reflectionCoefficient = r0 + (T{1} - r0) * x * x * x * x * x;
        //    *refractionCoefficient = T{1} - *reflectionCoefficient;
        //}

        inline constexpr T Length() const
        {
            return Math::sqrt(LengthSquared());
        }

        inline constexpr T LengthSquared() const
        {
            return (X * X) + (Y * Y) + (Z * Z);
        }

        inline constexpr Vector3T Max(const Vector3T& other) const
        {
            return Vector3T{
                Math::max(X, other.X),
                Math::max(Y, other.Y),
                Math::max(Z, other.Z),
            };
        }

        inline constexpr Vector3T Min(const Vector3T& other) const
        {
            return Vector3T{
                Math::min(X, other.X),
                Math::min(Y, other.Y),
                Math::min(Z, other.Z),
            };
        }

        inline constexpr Vector3T& Normalize()
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

            return *this;
        }

        inline constexpr Vector3T NormalizeConst() const
        {
            Vector3T result = *this;
            return result.Normalize();
        }

        inline constexpr Vector3T ProjectOnto(const Vector3T& vectorToProjectOnto) const
        {
            return vectorToProjectOnto * ((*this * vectorToProjectOnto) / (vectorToProjectOnto * vectorToProjectOnto));
        }

        inline constexpr Vector3T& Reciprical()
        {
            X = Math::rcp(X);
            Y = Math::rcp(Y);
            Z = Math::rcp(Z);

            return *this;
        }

        inline constexpr Vector3T RecipricalConst() const
        {
            Vector3T result = *this;
            result.Reciprical();

            return result;
        }

        inline constexpr Vector3T Reflect(const Vector3T& normal) const
        {
            return *this - 2.0f * (*this * normal) * normal;
        }

        static constexpr Vector3T Refract(const Vector3T& incomingDirection, const Vector3T& normal, T fromIndex, T toIndex)
        {
            T n = fromIndex / toIndex;
            T cos = -(incomingDirection * normal);

            T underSquareRoot = T{1} - n * n * (T{1} - cos * cos);
            if (underSquareRoot < T{0})
            {
                return Vector3T{};
            }

            return (n * incomingDirection) - (n * cos - Math::sqrt(underSquareRoot)) * normal;
        }

        static constexpr T SchlickApproximation(const Vector3T& incomingDirection, const Vector3T& normal, T fromIndex, T toIndex)
        {
            T r = (fromIndex - toIndex) / (fromIndex + toIndex);
            r = r * r;

            T cos = -(incomingDirection * normal);
            T x = T{1} - cos;

            return r + (T{1} - r) * x * x * x * x * x;
        }

        inline constexpr T& operator[](size_t index)
        {
            switch (index)
            {
                case 0: return X;
                case 1: return Y;
                case 2: return Z;
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
                default: return X;
            }
        }

        inline constexpr Vector3T operator+() const
        {
            return {+X, +Y, +Z};
        }

        inline constexpr Vector3T operator-() const
        {
            return {-X, -Y, -Z};
        }

        inline constexpr Vector3T& operator++()
        {
            X += T{1};
            Y += T{1};
            Z += T{1};

            return *this;
        }

        inline constexpr Vector3T& operator--()
        {
            X -= T{1};
            Y -= T{1};
            Z -= T{1};

            return *this;
        }

        inline constexpr Vector3T operator++(int)
        {
            Vector3T temp = *this;

            X += T{1};
            Y += T{1};
            Z += T{1};

            return temp;
        }

        inline constexpr Vector3T operator--(int)
        {
            Vector3T temp = *this;

            X -= T{1};
            Y -= T{1};
            Z -= T{1};

            return temp;
        }

        inline constexpr Vector3T operator+(const Vector3T& right) const
        {
            return Vector3T{X + right.X, Y + right.Y, Z + right.Z};
        }

        inline constexpr Vector3T operator-(const Vector3T& right) const
        {
            return Vector3T{X - right.X, Y - right.Y, Z - right.Z};
        }

        inline constexpr T operator*(const Vector3T& right) const
        {
            return Dot(right);
        }

        inline constexpr Vector3T operator%(const Vector3T& right) const
        {
            return CrossProduct(right);
        }

        inline constexpr Vector3T operator+(T right) const
        {
            return Vector3T{X + right, Y + right, Z + right};
        }

        inline constexpr Vector3T operator-(T right) const
        {
            return Vector3T{X - right, Y - right, Z - right};
        }

        inline constexpr Vector3T operator*(T right) const
        {
            return Vector3T{X * right, Y * right, Z * right};
        }

        inline constexpr Vector3T operator/(T right) const
        {
            return Vector3T{X / right, Y / right, Z / right};
        }

        inline constexpr Vector3T& operator+=(const Vector3T& other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;

            return *this;
        }

        inline constexpr Vector3T& operator-=(const Vector3T& other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;

            return *this;
        }

        inline constexpr Vector3T& operator+=(T other)
        {
            X += other;
            Y += other;
            Z += other;

            return *this;
        }

        inline constexpr Vector3T& operator-=(T other)
        {
            X -= other;
            Y -= other;
            Z -= other;

            return *this;
        }

        inline constexpr Vector3T& operator*=(T other)
        {
            X *= other;
            Y *= other;
            Z *= other;

            return *this;
        }

        inline constexpr Vector3T& operator/=(T other)
        {
            X /= other;
            Y /= other;
            Z /= other;

            return *this;
        }

        template <any_number U>
        inline constexpr explicit operator Vector3T<U>() const
        {
            return Vector3T<U>(static_cast<U>(X), static_cast<U>(Y), static_cast<U>(Z));
        }
    };

    export template <any_number T>
        inline constexpr Vector3T<T> operator+(T left, const Vector3T<T>& right)
    {
        return {left + right.X, left + right.Y, left + right.Z};
    }

    export template <any_number T>
        inline constexpr Vector3T<T> operator*(T left, const Vector3T<T>& right)
    {
        return {left * right.X, left * right.Y, left * right.Z};
    }

    export using Vector3 = Vector3T<real>;
    export using FloatVector3 = Vector3T<float>;
    export using DoubleVector3 = Vector3T<double>;
    export using IntVector3 = Vector3T<int>;
    export using UIntVector3 = Vector3T<unsigned int>;
}