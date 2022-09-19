module;

export module Vector2;

import <cmath>;

import Math;

namespace Yart
{
	export template <typename T>
		requires std::integral<T> || std::floating_point<T>
	class __declspec(dllexport) alignas(sizeof(T) * 2) Vector2T
	{
	public:
		T X{0};
		T Y{0};

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

		inline constexpr T Distance(const Vector2T& right)
		{
			return Math::sqrt(DistanceSquared(right));
		}

		inline constexpr T DistanceSquared(const Vector2T& right)
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
	};

	export template <typename T>
		requires std::integral<T> || std::floating_point<T>
	inline constexpr Vector2T<T> operator+(T left, const Vector2T<T>&right)
	{
		return {left + right.X, left + right.Y};
	}

	export template <typename T>
		requires std::integral<T> || std::floating_point<T>
	inline constexpr Vector2T<T> operator*(T left, const Vector2T<T>&right)
	{
		return {left * right.X, left * right.Y};
	}

	export using Vector2 = Vector2T<float>;
	export using IntVector2 = Vector2T<int>;
	export using UIntVector2 = Vector2T<unsigned int>;

	static_assert(Vector2{0, -5}.Abs().Normalize().Length() == 1);
}