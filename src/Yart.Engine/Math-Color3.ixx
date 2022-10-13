export module Math:Color3;

import "Common.h";

import :Basics;
import :Color3Decl;
import :Vector3Decl;

namespace Yart
{
    export template <any_number T>
        class __declspec(dllexport) alignas(sizeof(T) * 4) Color3T
    {
    public:
        T R{};
        T G{};
        T B{};

        inline constexpr Color3T() = default;

        inline constexpr explicit Color3T(T scalar)
            : R{scalar}, G{scalar}, B{scalar}
        {

        }

        inline constexpr Color3T(T r, T g, T b)
            : R{r}, G{g}, B{b}
        {

        }

        inline constexpr Color3T& Abs()
        {
            R = Math::abs(R);
            G = Math::abs(G);
            B = Math::abs(B);

            return *this;
        }

        static inline constexpr Color3T Abs(const Color3T& value)
        {
            Color3T result = value;
            return result.Abs();
        }

        inline constexpr Color3T& Exp()
        {
            R = Math::exp(R);
            G = Math::exp(G);
            B = Math::exp(B);

            return *this;
        }

        static inline constexpr Color3T Exp(const Color3T& value)
        {
            Color3T result = value;
            return result.Exp();
        }

        inline constexpr Color3T& Log()
        {
            R = Math::log(R);
            G = Math::log(G);
            B = Math::log(B);

            return *this;
        }

        static inline constexpr Color3T Log(const Color3T& value)
        {
            Color3T result = value;
            return result.Log();
        }

        static inline constexpr Color3T Max(const Color3T& left, const Color3T& right)
        {
            return Color3T{
                Math::max(left.R, right.R),
                Math::max(left.G, right.G),
                Math::max(left.B, right.B),
            };
        }

        static inline constexpr Color3T Min(const Color3T& left, const Color3T& right)
        {
            return Color3T{
                Math::min(left.R, right.R),
                Math::min(left.G, right.G),
                Math::min(left.B, right.B),
            };
        }

        inline constexpr Color3T& Reciprical()
        {
            R = Math::rcp(R);
            G = Math::rcp(G);
            B = Math::rcp(B);

            return *this;
        }

        static inline constexpr Color3T Reciprical(const Color3T& value)
        {
            Color3T result = value;
            return result.Reciprical();
        }

        inline constexpr T& operator[](size_t index)
        {
            switch (index)
            {
                case 0: return R;
                case 1: return G;
                case 2: return B;
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
                default: return R;
            }
        }

        inline constexpr Color3T operator+() const
        {
            return {+R, +G, +B};
        }

        inline constexpr Color3T operator-() const
        {
            return {-R, -G, -B};
        }

        inline constexpr Color3T& operator++()
        {
            R += T{1};
            G += T{1};
            B += T{1};

            return *this;
        }

        inline constexpr Color3T& operator--()
        {
            R -= T{1};
            G -= T{1};
            B -= T{1};

            return *this;
        }

        inline constexpr Color3T operator++(int)
        {
            Color3T temp = *this;

            R += T{1};
            G += T{1};
            B += T{1};

            return temp;
        }

        inline constexpr Color3T operator--(int)
        {
            Color3T temp = *this;

            R -= T{1};
            G -= T{1};
            B -= T{1};

            return temp;
        }

        inline constexpr Color3T operator+(const Color3T& right) const
        {
            return Color3T{R + right.R, G + right.G, B + right.B};
        }

        inline constexpr Color3T operator-(const Color3T& right) const
        {
            return Color3T{R - right.R, G - right.G, B - right.B};
        }

        inline constexpr Color3T operator*(const Color3T& right) const
        {
            return Color3T{R * right.R, G * right.G, B * right.B};
        }

        inline constexpr Color3T operator/(const Color3T& right) const
        {
            return Color3T{R / right.R, G / right.G, B / right.B};
        }

        inline constexpr Color3T operator+(T right) const
        {
            return Color3T{R + right, G + right, B + right};
        }

        inline constexpr Color3T operator-(T right) const
        {
            return Color3T{R - right, G - right, B - right};
        }

        inline constexpr Color3T operator*(T right) const
        {
            return Color3T{R * right, G * right, B * right};
        }

        inline constexpr Color3T operator/(T right) const
        {
            return Color3T{R / right, G / right, B / right};
        }

        inline constexpr Color3T& operator+=(const Color3T& other)
        {
            R += other.R;
            G += other.G;
            B += other.B;

            return *this;
        }

        inline constexpr Color3T& operator-=(const Color3T& other)
        {
            R -= other.R;
            G -= other.G;
            B -= other.B;

            return *this;
        }

        inline constexpr Color3T& operator*=(const Color3T& other)
        {
            R *= other.R;
            G *= other.G;
            B *= other.B;

            return *this;
        }

        inline constexpr Color3T& operator/=(const Color3T& other)
        {
            R /= other.R;
            G /= other.G;
            B /= other.B;

            return *this;
        }

        inline constexpr Color3T& operator+=(T other)
        {
            R += other;
            G += other;
            B += other;

            return *this;
        }

        inline constexpr Color3T& operator-=(T other)
        {
            R -= other;
            G -= other;
            B -= other;

            return *this;
        }

        inline constexpr Color3T& operator*=(T other)
        {
            R *= other;
            G *= other;
            B *= other;

            return *this;
        }

        inline constexpr Color3T& operator/=(T other)
        {
            R /= other;
            G /= other;
            B /= other;

            return *this;
        }

        inline constexpr explicit operator Vector3T<T>() const
        {
            return Vector3T<T>{R, G, B};
        }

        template <any_number U>
        inline constexpr explicit operator Color3T<U>() const
        {
            return Color3T<U>{static_cast<U>(R), static_cast<U>(G), static_cast<U>(B)};
        }
    };

    export template <any_number T>
        inline constexpr Color3T<T> operator+(T left, const Color3T<T>& right)
    {
        return {left + right.R, left + right.G, left + right.B};
    }

    export template <any_number T>
        inline constexpr Color3T<T> operator-(T left, const Color3T<T>& right)
    {
        return {left + right.R, left + right.G, left + right.B};
    }

    export template <any_number T>
        inline constexpr Color3T<T> operator*(T left, const Color3T<T>& right)
    {
        return {left * right.R, left * right.G, left * right.B};
    }

    export template <any_number T>
        inline constexpr Color3T<T> operator/(T left, const Color3T<T>& right)
    {
        return {left * right.R, left * right.G, left * right.B};
    }

    export using Color3 = Color3T<real>;
    export using FloatColor3 = Color3T<float>;
    export using DoubleColor3 = Color3T<double>;
    export using IntColor3 = Color3T<int>;
    export using UIntColor3 = Color3T<unsigned int>;
}