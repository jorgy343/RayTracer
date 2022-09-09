#include <cmath>
#include <concepts>

export module RayTracer.Vector2;

import RayTracer.Math;

namespace RayTracer
{
    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    class alignas(sizeof(T) * 2) Vector2T
    {
    public:
        T X{0};
        T Y{0};

        Vector2T() = default;

        explicit Vector2T(T scalar)
            : X{scalar}, Y{scalar}
        {

        }

        Vector2T(T x, T y)
            : X{x}, Y{y}
        {

        }

        inline Vector2T& Abs()
        {
            X = std::abs(X);
            Y = std::abs(Y);

            return *this;
        }

        inline bool Compare(const Vector2T& right, T maximumAllowedErrorPerComponent)
        {
            if constexpr (std::floating_point<T>)
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
                    (!std::isfinite(X) || !std::isfinite(right.X) || std::abs(X - right.X) < maximumAllowedErrorPerComponent) &&
                    (!std::isfinite(Y) || !std::isfinite(right.Y) || std::abs(Y - right.Y) < maximumAllowedErrorPerComponent);
            }
            else
            {
                return
                    X == right.X &&
                    Y == right.Y;
            }
        }

        inline Vector2T ComponentwiseMultiply(const Vector2T& right) const
        {
            return Vector2T
            {
                X * right.X,
                Y * right.Y,
            };
        }

        inline T Distance(const Vector2T& right)
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

        inline T DistanceSquared(const Vector2T& right)
        {
            T x = X - right.X;
            T y = Y - right.Y;

            return x * x + y * y;
        }

        inline T Dot(const Vector2T& right) const
        {
            return (X * right.X) + (Y * right.Y);
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
            return (X * X) + (Y * Y);
        }

        inline Vector2T& Normalize()
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

            return *this;
        }

        Vector2T operator+() const
        {
            return {+X, +Y};
        }

        Vector2T operator-() const
        {
            return {-X, -Y};
        }

        Vector2T& operator++()
        {
            X += T{1};
            Y += T{1};

            return *this;
        }

        Vector2T& operator--()
        {
            X -= T{1};
            Y -= T{1};

            return *this;
        }

        Vector2T operator++(int)
        {
            Vector2T temp = *this;

            X += T{1};
            Y += T{1};

            return temp;
        }

        Vector2T operator--(int)
        {
            Vector2T temp = *this;

            X -= T{1};
            Y -= T{1};

            return temp;
        }

        Vector2T operator+(const Vector2T& right) const
        {
            return Vector2T{X + right.X, Y + right.Y};
        }

        Vector2T operator-(const Vector2T& right) const
        {
            return Vector2T{X - right.X, Y - right.Y};
        }

        T operator*(const Vector2T& right) const
        {
            return Dot(right);
        }

        Vector2T operator+(T right) const
        {
            return Vector2T{X + right, Y + right};
        }

        Vector2T operator-(T right) const
        {
            return Vector2T{X - right, Y - right};
        }

        Vector2T operator*(T right) const
        {
            return Vector2T{X * right, Y * right};
        }

        Vector2T operator/(T right) const
        {
            return Vector2T{X / right, Y / right};
        }

        Vector2T& operator+=(const Vector2T& other)
        {
            X += other.X;
            Y += other.Y;

            return *this;
        }

        Vector2T& operator-=(const Vector2T& other)
        {
            X -= other.X;
            Y -= other.Y;

            return *this;
        }

        Vector2T& operator+=(T other)
        {
            X += other;
            Y += other;

            return *this;
        }

        Vector2T& operator-=(T other)
        {
            X -= other;
            Y -= other;

            return *this;
        }

        Vector2T& operator*=(T other)
        {
            X *= other;
            Y *= other;

            return *this;
        }

        Vector2T& operator/=(T other)
        {
            X /= other;
            Y /= other;

            return *this;
        }
    };

    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    Vector2T<T> operator+(T left, const Vector2T<T>& right)
    {
        return {left + right.X, left + right.Y};
    }

    export template <typename T>
        requires std::integral<T> || std::floating_point<T>
    Vector2T<T> operator*(T left, const Vector2T<T>& right)
    {
        return {left * right.X, left * right.Y};
    }

    export using Vector2 = Vector2T<float>;
    export using IntVector2 = Vector2T<int>;
    export using UIntVector2 = Vector2T<unsigned int>;
}