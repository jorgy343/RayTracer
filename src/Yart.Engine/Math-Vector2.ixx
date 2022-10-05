export module Math:Vector2;

import "Common.h";

import :Basics;

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

		inline constexpr bool Compare(const Vector2T& right, T maximumAllowedErrorPerComponent)
		{
			if constexpr (std::floating_point<T>)
			{
				bool areNansBad =
					Math::isnan(X) ^ Math::isnan(right.X) ||
					Math::isnan(Y) ^ Math::isnan(right.Y);

				if (areNansBad)
				{
					return false;
				}

				bool areInfinitiesBad =
					Math::isinf(X) ^ Math::isinf(right.X) ||
					Math::isinf(Y) ^ Math::isinf(right.Y);

				if (areInfinitiesBad)
				{
					return false;
				}

				return
					(!Math::isfinite(X) || !Math::isfinite(right.X) || Math::abs(X - right.X) < maximumAllowedErrorPerComponent) &&
					(!Math::isfinite(Y) || !Math::isfinite(right.Y) || Math::abs(Y - right.Y) < maximumAllowedErrorPerComponent);
			}
			else
			{
				return
					X == right.X &&
					Y == right.Y;
			}
		}

		inline constexpr Vector2T ComponentwiseMultiply(const Vector2T& right) const
		{
			return Vector2T
			{
				X * right.X,
				Y * right.Y,
			};
		}

		inline constexpr T Distance(const Vector2T& right) const
		{
			return Math::sqrt(DistanceSquared(right));
		}

		inline constexpr T DistanceSquared(const Vector2T& right) const
		{
			T x = X - right.X;
			T y = Y - right.Y;

			return x * x + y * y;
		}

		inline constexpr T Dot(const Vector2T& right) const
		{
			return (X * right.X) + (Y * right.Y);
		}

		inline constexpr T Length() const
		{
			return Math::sqrt(LengthSquared());
		}

		inline constexpr T LengthSquared() const
		{
			return (X * X) + (Y * Y);
		}

        inline constexpr Vector2T Max(const Vector2T& other) const
        {
            return Vector2T{
                Math::max(X, other.X),
                Math::max(Y, other.Y),
            };
        }

        inline constexpr Vector2T Min(const Vector2T& other) const
        {
            return Vector2T{
                Math::min(X, other.X),
                Math::min(Y, other.Y),
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

		inline constexpr Vector2T NormalizeConst() const
		{
			Vector2T result = *this;
			return result.Normalize();
		}

		inline constexpr Vector2T& Reciprical()
		{
			X = Math::rcp(X);
			Y = Math::rcp(Y);

			return *this;
		}

		inline constexpr Vector2T RecipricalConst() const
		{
			Vector2T result = *this;
			result.Reciprical();

			return result;
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