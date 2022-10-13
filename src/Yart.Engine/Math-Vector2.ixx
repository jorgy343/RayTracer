export module Math:Vector2;

import "Common.h";

import :Basics;
import :Vector2Decl;

namespace Yart
{
	export template <any_number T>
	class __declspec(dllexport) alignas(sizeof(T) * 2) Vector2T
	{
	public:
		T X{};
		T Y{};

		inline constexpr Vector2T() = default;

		inline constexpr explicit Vector2T(T scalar)
			: X{scalar}, Y{scalar}
		{

		}

		inline constexpr Vector2T(T x, T y)
			: X{x}, Y{y}
		{

		}

		inline constexpr Vector2T& Abs()
		{
			X = Math::abs(X);
			Y = Math::abs(Y);

			return *this;
		}

        static inline constexpr Vector2T Abs(const Vector2T& value)
        {
            Vector2T result = value;
            return result.Abs();
        }

        static inline constexpr bool Compare(const Vector2T& left, const Vector2T& right, T maximumAllowedErrorPerComponent)
        {
            if constexpr (std::floating_point<T>)
            {
                bool areNansBad =
                    Math::isnan(left.X) ^ Math::isnan(right.X) ||
                    Math::isnan(left.Y) ^ Math::isnan(right.Y);

                if (areNansBad)
                {
                    return false;
                }

                bool areInfinitiesBad =
                    Math::isinf(left.X) ^ Math::isinf(right.X) ||
                    Math::isinf(left.Y) ^ Math::isinf(right.Y);

                if (areInfinitiesBad)
                {
                    return false;
                }

                return
                    (!Math::isfinite(left.X) || !Math::isfinite(right.X) || Math::abs(left.X - right.X) < maximumAllowedErrorPerComponent) &&
                    (!Math::isfinite(left.Y) || !Math::isfinite(right.Y) || Math::abs(left.Y - right.Y) < maximumAllowedErrorPerComponent);
            }
            else
            {
                return
                    left.X == right.X &&
                    left.Y == right.Y;
            }
        }

        static inline constexpr Vector2T ComponentwiseMultiply(const Vector2T& left, const Vector2T& right)
        {
            return Vector2T
            {
                left.X * right.X,
                left.Y * right.Y,
            };
        }

        static inline constexpr T Distance(const Vector2T& left, const Vector2T& right)
        {
            return Math::sqrt(DistanceSquared(left, right));
        }

        static inline constexpr T DistanceSquared(const Vector2T& left, const Vector2T& right)
        {
            T x = left.X - right.X;
            T y = left.Y - right.Y;

            return x * x + y * y;
        }

        static inline constexpr T Dot(const Vector2T& left, const Vector2T& right)
        {
            return (left.X * right.X) + (left.Y * right.Y);
        }

        inline constexpr Vector2T& Exp()
        {
            X = Math::exp(X);
            Y = Math::exp(Y);

            return *this;
        }

        static inline constexpr Vector2T Exp(const Vector2T& value)
        {
            Vector2T result = value;
            return result.Exp();
        }

        inline constexpr T Length() const
        {
            return Math::sqrt(LengthSquared());
        }

        inline constexpr T LengthSquared() const
        {
            return (X * X) + (Y * Y);
        }

        inline constexpr Vector2T& Log()
        {
            X = Math::log(X);
            Y = Math::log(Y);

            return *this;
        }

        static inline constexpr Vector2T Log(const Vector2T& value)
        {
            Vector2T result = value;
            return result.Log();
        }

        static inline constexpr Vector2T Max(const Vector2T& left, const Vector2T& right)
        {
            return Vector2T{
                Math::max(left.X, right.X),
                Math::max(left.Y, right.Y),
            };
        }

        static inline constexpr Vector2T Min(const Vector2T& left, const Vector2T& right)
        {
            return Vector2T{
                Math::min(left.X, right.X),
                Math::min(left.Y, right.Y),
            };
        }

		inline constexpr Vector2T& Normalize()
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

			return *this;
		}

        static inline constexpr Vector2T Normalize(const Vector2T& value)
        {
            Vector2T result = value;
            return result.Normalize();
        }

		inline constexpr Vector2T& Reciprical()
		{
			X = Math::rcp(X);
			Y = Math::rcp(Y);

			return *this;
		}

        static inline constexpr Vector2T Reciprical(const Vector2T& value)
        {
            Vector2T result = value;
            return result.Reciprical();
        }

        inline constexpr T& operator[](size_t index)
        {
            switch (index)
            {
                case 0: return X;
                case 1: return Y;
                default: return X;
            }
        }

        inline constexpr const T& operator[](size_t index) const
        {
            switch (index)
            {
                case 0: return X;
                case 1: return Y;
                default: return X;
            }
        }

		inline constexpr Vector2T operator+() const
		{
			return {+X, +Y};
		}

		inline constexpr Vector2T operator-() const
		{
			return {-X, -Y};
		}

		inline constexpr Vector2T& operator++()
		{
			X += T{1};
			Y += T{1};

			return *this;
		}

		inline constexpr Vector2T& operator--()
		{
			X -= T{1};
			Y -= T{1};

			return *this;
		}

		inline constexpr Vector2T operator++(int)
		{
			Vector2T temp = *this;

			X += T{1};
			Y += T{1};

			return temp;
		}

		inline constexpr Vector2T operator--(int)
		{
			Vector2T temp = *this;

			X -= T{1};
			Y -= T{1};

			return temp;
		}

		inline constexpr Vector2T operator+(const Vector2T& right) const
		{
			return Vector2T{X + right.X, Y + right.Y};
		}

		inline constexpr Vector2T operator-(const Vector2T& right) const
		{
			return Vector2T{X - right.X, Y - right.Y};
		}

		inline constexpr T operator*(const Vector2T& right) const
		{
			return Dot(right);
		}

		inline constexpr Vector2T operator+(T right) const
		{
			return Vector2T{X + right, Y + right};
		}

		inline constexpr Vector2T operator-(T right) const
		{
			return Vector2T{X - right, Y - right};
		}

		inline constexpr Vector2T operator*(T right) const
		{
			return Vector2T{X * right, Y * right};
		}

		inline constexpr Vector2T operator/(T right) const
		{
			return Vector2T{X / right, Y / right};
		}

		inline constexpr Vector2T& operator+=(const Vector2T& other)
		{
			X += other.X;
			Y += other.Y;

			return *this;
		}

		inline constexpr Vector2T& operator-=(const Vector2T& other)
		{
			X -= other.X;
			Y -= other.Y;

			return *this;
		}

		inline constexpr Vector2T& operator+=(T other)
		{
			X += other;
			Y += other;

			return *this;
		}

		inline constexpr Vector2T& operator-=(T other)
		{
			X -= other;
			Y -= other;

			return *this;
		}

		inline constexpr Vector2T& operator*=(T other)
		{
			X *= other;
			Y *= other;

			return *this;
		}

		inline constexpr Vector2T& operator/=(T other)
		{
			X /= other;
			Y /= other;

			return *this;
		}

        template <any_number U>
        inline constexpr explicit operator Vector2T<U>() const
        {
            return Vector2T<U>(static_cast<U>(X), static_cast<U>(Y));
        }
	};

	export template <any_number T>
	inline constexpr Vector2T<T> operator+(T left, const Vector2T<T>&right)
	{
		return {left + right.X, left + right.Y};
	}

	export template <any_number T>
	inline constexpr Vector2T<T> operator*(T left, const Vector2T<T>&right)
	{
		return {left * right.X, left * right.Y};
	}

	export using Vector2 = Vector2T<real>;
    export using FloatVector2 = Vector2T<float>;
    export using DoubleVector2 = Vector2T<double>;
	export using IntVector2 = Vector2T<int>;
	export using UIntVector2 = Vector2T<unsigned int>;
}