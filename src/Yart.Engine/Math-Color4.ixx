export module Math:Color4;

import "Common.h";

import :Basics;
import :Color3;
import :Color4Decl;
import :Vector4Decl;

namespace Yart
{
    export template <any_number T>
        class __declspec(dllexport) alignas(sizeof(T) * 4) Color4T
    {
    public:
        T R{};
        T G{};
        T B{};
        T A{};

        inline constexpr Color4T() = default;

        inline constexpr explicit Color4T(T scalar)
            : R{scalar}, G{scalar}, B{scalar}, A{scalar}
        {

        }

        inline constexpr Color4T(T r, T g, T b, T a)
            : R{r}, G{g}, B{b}, A{a}
        {

        }

        inline constexpr Color4T(const Color3T<T>& color3, T a)
            : R{color3.R}, G{color3.G}, B{color3.B}, A{a}
        {

        }

        inline constexpr Color4T& Abs()
        {
            R = Math::abs(R);
            G = Math::abs(G);
            B = Math::abs(B);
            A = Math::abs(A);

            return *this;
        }

        static inline constexpr Color4T Abs(const Color4T& value)
        {
            Color4T result = value;
            return result.Abs();
        }

        inline constexpr Color4T& Exp()
        {
            R = Math::exp(R);
            G = Math::exp(G);
            B = Math::exp(B);
            A = Math::exp(A);

            return *this;
        }

        static inline constexpr Color4T Exp(const Color4T& value)
        {
            Color4T result = value;
            return result.Exp();
        }

        inline constexpr Color4T& Log()
        {
            R = Math::log(R);
            G = Math::log(G);
            B = Math::log(B);
            A = Math::log(A);

            return *this;
        }

        static inline constexpr Color4T Log(const Color4T& value)
        {
            Color4T result = value;
            return result.Log();
        }

        static inline constexpr Color4T Max(const Color4T& left, const Color4T& right)
        {
            return Color4T{
                Math::max(left.R, right.R),
                Math::max(left.G, right.G),
                Math::max(left.B, right.B),
                Math::max(left.A, right.A),
            };
        }

        static inline constexpr Color4T Min(const Color4T& left, const Color4T& right)
        {
            return Color4T{
                Math::min(left.R, right.R),
                Math::min(left.G, right.G),
                Math::min(left.B, right.B),
                Math::min(left.A, right.A),
            };
        }

        inline constexpr Color4T& Reciprical()
        {
            R = Math::rcp(R);
            G = Math::rcp(G);
            B = Math::rcp(B);
            A = Math::rcp(A);

            return *this;
        }

        static inline constexpr Color4T Reciprical(const Color4T& value)
        {
            Color4T result = value;
            return result.Reciprical();
        }

        inline constexpr T& operator[](size_t index)
        {
            switch (index)
            {
                case 0: return R;
                case 1: return G;
                case 2: return B;
                case 3: return A;
                default: return R;
            }
        }

        inline constexpr const T& operator[](size_t index) const
        {
            switch (index)
            {
                case 0: return R;
                case 1: return G;
                case 2: return B;
                case 3: return A;
                default: return R;
            }
        }

        inline constexpr Color4T operator+() const
        {
            return {+R, +G, +B, +A};
        }

        inline constexpr Color4T operator-() const
        {
            return {-R, -G, -B, -A};
        }

        inline constexpr Color4T& operator++()
        {
            R += T{1};
            G += T{1};
            B += T{1};
            A += T{1};

            return *this;
        }

        inline constexpr Color4T& operator--()
        {
            R -= T{1};
            G -= T{1};
            B -= T{1};
            A -= T{1};

            return *this;
        }

        inline constexpr Color4T operator++(int)
        {
            Color4T temp = *this;

            R += T{1};
            G += T{1};
            B += T{1};
            A += T{1};

            return temp;
        }

        inline constexpr Color4T operator--(int)
        {
            Color4T temp = *this;

            R -= T{1};
            G -= T{1};
            B -= T{1};
            A -= T{1};

            return temp;
        }

        inline constexpr Color4T operator+(const Color4T& right) const
        {
            return Color4T{R + right.R, G + right.G, B + right.B, A + right.A};
        }

        inline constexpr Color4T operator-(const Color4T& right) const
        {
            return Color4T{R - right.R, G - right.G, B - right.B, A - right.A};
        }

        inline constexpr Color4T operator*(const Color4T& right) const
        {
            return Color4T{R * right.R, G * right.G, B * right.B, A * right.A};
        }

        inline constexpr Color4T operator/(const Color4T& right) const
        {
            return Color4T{R / right.R, G / right.G, B / right.B, A / right.A};
        }

        inline constexpr Color4T operator+(T right) const
        {
            return Color4T{R + right, G + right, B + right, A + right};
        }

        inline constexpr Color4T operator-(T right) const
        {
            return Color4T{R - right, G - right, B - right, A - right};
        }

        inline constexpr Color4T operator*(T right) const
        {
            return Color4T{R * right, G * right, B * right, A * right};
        }

        inline constexpr Color4T operator/(T right) const
        {
            return Color4T{R / right, G / right, B / right, A / right};
        }

        inline constexpr Color4T& operator+=(const Color4T& other)
        {
            R += other.R;
            G += other.G;
            B += other.B;
            A += other.A;

            return *this;
        }

        inline constexpr Color4T& operator-=(const Color4T& other)
        {
            R -= other.R;
            G -= other.G;
            B -= other.B;
            A -= other.A;

            return *this;
        }

        inline constexpr Color4T& operator*=(const Color4T& other)
        {
            R *= other.R;
            G *= other.G;
            B *= other.B;
            A *= other.A;

            return *this;
        }

        inline constexpr Color4T& operator/=(const Color4T& other)
        {
            R /= other.R;
            G /= other.G;
            B /= other.B;
            A /= other.A;

            return *this;
        }

        inline constexpr Color4T& operator+=(T other)
        {
            R += other;
            G += other;
            B += other;
            A += other;

            return *this;
        }

        inline constexpr Color4T& operator-=(T other)
        {
            R -= other;
            G -= other;
            B -= other;
            A -= other;

            return *this;
        }

        inline constexpr Color4T& operator*=(T other)
        {
            R *= other;
            G *= other;
            B *= other;
            A *= other;

            return *this;
        }

        inline constexpr Color4T& operator/=(T other)
        {
            R /= other;
            G /= other;
            B /= other;
            A /= other;

            return *this;
        }

        inline constexpr explicit operator Vector4T<T>() const
        {
            return Vector4T<T>{R, G, B, A};
        }

        template <any_number U>
        inline constexpr explicit operator Color4T<U>() const
        {
            return Color4T<U>{static_cast<U>(R), static_cast<U>(G), static_cast<U>(B), static_cast<U>(A)};
        }
    };

    export template <any_number T>
        inline constexpr Color4T<T> operator+(T left, const Color4T<T>& right)
    {
        return {left + right.R, left + right.G, left + right.B, left + right.A};
    }

    export template <any_number T>
        inline constexpr Color4T<T> operator-(T left, const Color4T<T>& right)
    {
        return {left + right.R, left + right.G, left + right.B, left + right.A};
    }

    export template <any_number T>
        inline constexpr Color4T<T> operator*(T left, const Color4T<T>& right)
    {
        return {left * right.R, left * right.G, left * right.B, left * right.A};
    }

    export template <any_number T>
        inline constexpr Color4T<T> operator/(T left, const Color4T<T>& right)
    {
        return {left * right.R, left * right.G, left * right.B, left * right.A};
    }

    export using Color4 = Color4T<real>;
    export using FloatColor4 = Color4T<float>;
    export using DoubleColor4 = Color4T<double>;
    export using IntColor4 = Color4T<int>;
    export using UIntColor4 = Color4T<unsigned int>;
}