module;

#include "Vcl.h"

export module Math:VectorVec3;

import :Vector3;

namespace Yart
{
    export template<vec_type T>
        class VectorVec3
    {
    public:
        T X{};
        T Y{};
        T Z{};

        inline VectorVec3() = default;

        inline VectorVec3(T value)
            : X{value}, Y{value}, Z{value}
        {

        }

        inline VectorVec3(T x, T y, T z)
            : X{x}, Y{y}, Z{z}
        {

        }

        inline VectorVec3(float value)
            requires std::same_as<vcl::Vec4f, T> || std::same_as<vcl::Vec8f, T>
        : X{value}, Y{value}, Z{value}
        {

        }

        inline VectorVec3(float x, float y, float z)
            requires std::same_as<vcl::Vec4f, T> || std::same_as<vcl::Vec8f, T>
        : X{x}, Y{y}, Z{z}
        {

        }

        inline VectorVec3(const float* x, const float* y, const float* z)
            requires std::same_as<vcl::Vec4f, T> || std::same_as<vcl::Vec8f, T>
        : X{T{}.load(x)}, Y{T{}.load(y)}, Z{T{}.load(z)}
        {

        }

        inline VectorVec3(Vector3T<float> value)
            requires std::same_as<vcl::Vec4f, T> || std::same_as<vcl::Vec8f, T>
        : X{value.X}, Y{value.Y}, Z{value.Z}
        {

        }

        inline VectorVec3(double value)
            requires std::same_as<vcl::Vec2d, T> || std::same_as<vcl::Vec4d, T>
        : X{value}, Y{value}, Z{value}
        {

        }

        inline VectorVec3(double x, double y, double z)
            requires std::same_as<vcl::Vec2d, T> || std::same_as<vcl::Vec4d, T>
        : X{x}, Y{y}, Z{z}
        {

        }

        inline VectorVec3(const double* x, const double* y, const double* z)
            requires std::same_as<vcl::Vec2d, T> || std::same_as<vcl::Vec4d, T>
        : X{T{}.load(x)}, Y{T{}.load(y)}, Z{T{}.load(z)}
        {

        }

        inline VectorVec3(Vector3T<double> value)
            requires std::same_as<vcl::Vec2d, T> || std::same_as<vcl::Vec4d, T>
        : X{value.X}, Y{value.Y}, Z{value.Z}
        {

        }

        static inline T Dot(VectorVec3 left, VectorVec3 right)
        {
            return mul_add(left.X, right.X, mul_add(left.Y, right.Y, left.Z * right.Z));
        }

        static inline VectorVec3 Cross(VectorVec3 left, VectorVec3 right)
        {
            return
            {
                left.Y * right.Z - left.Z * right.Y,
                left.Z * right.X - left.X * right.Z,
                left.X * right.Y - left.Y * right.X,
            };
        }

        inline VectorVec3 operator+() const
        {
            return {+X, +Y, +Z};
        }

        inline VectorVec3 operator-() const
        {
            return {-X, -Y, -Z};
        }

        inline VectorVec3& operator++()
        {
            X += T{1};
            Y += T{1};
            Z += T{1};

            return *this;
        }

        inline VectorVec3& operator--()
        {
            X -= T{1};
            Y -= T{1};
            Z -= T{1};

            return *this;
        }

        inline VectorVec3 operator++(int)
        {
            VectorVec3 temp = *this;

            X += T{1};
            Y += T{1};
            Z += T{1};

            return temp;
        }

        inline VectorVec3 operator--(int)
        {
            VectorVec3 temp = *this;

            X -= T{1};
            Y -= T{1};
            Z -= T{1};

            return temp;
        }

        inline VectorVec3 operator+(const VectorVec3& right) const
        {
            return VectorVec3{X + right.X, Y + right.Y, Z + right.Z};
        }

        inline VectorVec3 operator-(const VectorVec3& right) const
        {
            return VectorVec3{X - right.X, Y - right.Y, Z - right.Z};
        }

        inline VectorVec3 operator%(const VectorVec3& right) const
        {
            return Cross(*this, right);
        }

        inline VectorVec3 operator+(T right) const
        {
            return VectorVec3{X + right, Y + right, Z + right};
        }

        inline VectorVec3 operator-(T right) const
        {
            return VectorVec3{X - right, Y - right, Z - right};
        }

        inline VectorVec3 operator*(T right) const
        {
            return VectorVec3{X * right, Y * right, Z * right};
        }

        inline VectorVec3 operator/(T right) const
        {
            return VectorVec3{X / right, Y / right, Z / right};
        }

        inline VectorVec3& operator+=(const VectorVec3& other)
        {
            X += other.X;
            Y += other.Y;
            Z += other.Z;

            return *this;
        }

        inline VectorVec3& operator-=(const VectorVec3& other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;

            return *this;
        }

        inline VectorVec3& operator+=(T other)
        {
            X += other;
            Y += other;
            Z += other;

            return *this;
        }

        inline VectorVec3& operator-=(T other)
        {
            X -= other;
            Y -= other;
            Z -= other;

            return *this;
        }

        inline VectorVec3& operator*=(T other)
        {
            X *= other;
            Y *= other;
            Z *= other;

            return *this;
        }

        inline VectorVec3& operator/=(T other)
        {
            X /= other;
            Y /= other;
            Z /= other;

            return *this;
        }
    };

    export template <vec_type T>
        inline constexpr VectorVec3<T> operator+(T left, const VectorVec3<T>& right)
    {
        return {left + right.X, left + right.Y, left + right.Z};
    }

    export template <vec_type T>
        inline constexpr VectorVec3<T> operator*(T left, const VectorVec3<T>& right)
    {
        return {left * right.X, left * right.Y, left * right.Z};
    }
}