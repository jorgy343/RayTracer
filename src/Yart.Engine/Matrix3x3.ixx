module;

#include "Vcl.h"

export module Matrix3x3;

import <cmath>;

import "Common.h";

import Math;
import Vector3;

using namespace vcl;

namespace Yart
{
    export template <real_number T>
    class __declspec(dllexport) alignas(64) Matrix3x3T
    {
    public:
        T M11{}, M12{}, M13{};
    private:
        T __DummyM14{};
    public:
        T M21{}, M22{}, M23{};
    private:
        T __DummyM24{};
    public:
        T M31{}, M32{}, M33{};
    private:
        T __DummyM34{};

    public:
        inline constexpr Matrix3x3T() = default;

        inline constexpr explicit Matrix3x3T(T scalar)
            :
            M11{scalar}, M12{scalar}, M13{scalar},
            M21{scalar}, M22{scalar}, M23{scalar},
            M31{scalar}, M32{scalar}, M33{scalar}
        {

        }

        inline constexpr Matrix3x3T(
            T m11, T m12, T m13,
            T m21, T m22, T m23,
            T m31, T m32, T m33)
            :
            M11{m11}, M12{m12}, M13{m13},
            M21{m21}, M22{m22}, M23{m23},
            M31{m31}, M32{m32}, M33{m33}
        {

        }

        inline constexpr Matrix3x3T(
            const Vector3T<T>& row1,
            const Vector3T<T>& row2,
            const Vector3T<T>& row3)
            :
            M11{row1.X}, M12{row1.Y}, M13{row1.Z},
            M21{row2.X}, M22{row2.Y}, M23{row2.Z},
            M31{row3.X}, M32{row3.Y}, M33{row3.Z}
        {

        }

        constexpr T Determinant() const
        {
            return
                (M11 * M22 * M33) +
                (M12 * M23 * M13) +
                (M13 * M21 * M32) -
                (M13 * M22 * M32) -
                (M12 * M21 * M33) -
                (M11 * M23 * M32);
        }

        constexpr Matrix3x3T& Invert()
        {
            // Source: https://stackoverflow.com/a/18504573/1078268

            T det =
                M11 * (M22 * M33 - M32 * M23) -
                M12 * (M21 * M33 - M23 * M31) +
                M13 * (M21 * M32 - M22 * M31);

            T invDet = Math::rcp(det);

            T m11 = (M22 * M33 - M32 * M23) * invDet;
            T m12 = (M13 * M32 - M12 * M33) * invDet;
            T m13 = (M12 * M23 - M13 * M22) * invDet;

            T m21 = (M23 * M31 - M21 * M33) * invDet;
            T m22 = (M11 * M33 - M13 * M31) * invDet;
            T m23 = (M21 * M13 - M11 * M23) * invDet;

            T m31 = (M21 * M32 - M31 * M22) * invDet;
            T m32 = (M31 * M12 - M11 * M32) * invDet;
            T m33 = (M11 * M22 - M21 * M12) * invDet;

            M11 = m11;
            M12 = m12;
            M13 = m13;

            M21 = m21;
            M22 = m22;
            M23 = m23;

            M31 = m31;
            M32 = m32;
            M33 = m33;

            return *this;
        }

        inline constexpr Matrix3x3T& Transpose()
        {
            M11 = M11;
            M12 = M21;
            M13 = M31;

            M21 = M12;
            M22 = M22;
            M23 = M32;

            M31 = M13;
            M32 = M23;
            M33 = M33;

            return *this;
        }

        inline constexpr T& operator[](size_t index)
        {
            switch (index)
            {
                case 0: return M11;
                case 1: return M12;
                case 2: return M13;

                case 3: return M21;
                case 4: return M22;
                case 5: return M23;

                case 6: return M31;
                case 7: return M32;
                case 8: return M33;

                default: return M11;
            }
        }

        inline constexpr const T& operator[](size_t index) const
        {
            switch (index)
            {
                case 0: return M11;
                case 1: return M12;
                case 2: return M13;

                case 3: return M21;
                case 4: return M22;
                case 5: return M23;

                case 6: return M31;
                case 7: return M32;
                case 8: return M33;

                default: return M11;
            }
        }

        inline constexpr Matrix3x3T operator+() const
        {
            return {
                +M11,
                +M12,
                +M13,

                +M21,
                +M22,
                +M23,

                +M31,
                +M32,
                +M33,
            };
        }

        inline constexpr Matrix3x3T operator-() const
        {
            return {
                -M11,
                -M12,
                -M13,

                -M21,
                -M22,
                -M23,

                -M31,
                -M32,
                -M33,
            };
        }

        inline constexpr Matrix3x3T& operator++()
        {
            M11 += T{1};
            M12 += T{1};
            M13 += T{1};

            M21 += T{1};
            M22 += T{1};
            M23 += T{1};

            M31 += T{1};
            M32 += T{1};
            M33 += T{1};

            return *this;
        }

        inline constexpr Matrix3x3T& operator--()
        {
            M11 -= T{1};
            M12 -= T{1};
            M13 -= T{1};

            M21 -= T{1};
            M22 -= T{1};
            M23 -= T{1};

            M31 -= T{1};
            M32 -= T{1};
            M33 -= T{1};

            return *this;
        }

        inline constexpr Matrix3x3T operator++(int)
        {
            Matrix3x3T temp = *this;

            M11 += T{1};
            M12 += T{1};
            M13 += T{1};

            M21 += T{1};
            M22 += T{1};
            M23 += T{1};

            M31 += T{1};
            M32 += T{1};
            M33 += T{1};

            return temp;
        }

        inline constexpr Matrix3x3T operator--(int)
        {
            Matrix3x3T temp = *this;

            M11 -= T{1};
            M12 -= T{1};
            M13 -= T{1};

            M21 -= T{1};
            M22 -= T{1};
            M23 -= T{1};

            M31 -= T{1};
            M32 -= T{1};
            M33 -= T{1};

            return temp;
        }

        inline constexpr Matrix3x3T operator+(const Matrix3x3T& right) const
        {
            return {
                M11 + right.M11,
                M12 + right.M12,
                M13 + right.M13,

                M21 + right.M21,
                M22 + right.M22,
                M23 + right.M23,

                M31 + right.M31,
                M32 + right.M32,
                M33 + right.M33,
            };
        }

        inline constexpr Matrix3x3T operator-(const Matrix3x3T& right) const
        {
            return {
                M11 - right.M11,
                M12 - right.M12,
                M13 - right.M13,

                M21 - right.M21,
                M22 - right.M22,
                M23 - right.M23,

                M31 - right.M31,
                M32 - right.M32,
                M33 - right.M33,
            };
        }

        inline constexpr Matrix3x3T operator*(const Matrix3x3T& right) const
        {
            if (std::is_constant_evaluated())
            {
                return {
                    M11 * right.M11 + M12 * right.M21 + M13 * right.M31,
                    M11 * right.M12 + M12 * right.M22 + M13 * right.M32,
                    M11 * right.M13 + M12 * right.M23 + M13 * right.M33,

                    M21 * right.M11 + M22 * right.M21 + M23 * right.M31,
                    M21 * right.M12 + M22 * right.M22 + M23 * right.M32,
                    M21 * right.M13 + M22 * right.M23 + M23 * right.M33,

                    M31 * right.M11 + M32 * right.M21 + M33 * right.M31,
                    M31 * right.M12 + M32 * right.M22 + M33 * right.M32,
                    M31 * right.M13 + M32 * right.M23 + M33 * right.M33,
                };
            }
            else if constexpr (std::same_as<float, T>)
            {
                // Currently this is slower on zen 2 architecture.
                //Vec4f rightColumn1 = gather4f<0, 4, 8, 12>(&right.M11);
                //Vec4f rightColumn2 = gather4f<1, 5, 9, 13>(&right.M11);
                //Vec4f rightColumn3 = gather4f<2, 6, 10, 14>(&right.M11);

                Vec4f rightColumn1{right.M11, right.M12, right.M13, right.__DummyM14};
                Vec4f rightColumn2{right.M21, right.M22, right.M23, right.__DummyM24};
                Vec4f rightColumn3{right.M31, right.M32, right.M33, right.__DummyM34};

                Vec4f leftRow11{M11};
                Vec4f leftRow21{M12};
                Vec4f leftRow31{M13};

                Vec4f resultRow1 = (leftRow11 * rightColumn1) + (leftRow21 * rightColumn2) + (leftRow31 * rightColumn3);

                Vec4f leftRow12{M21};
                Vec4f leftRow22{M22};
                Vec4f leftRow32{M23};

                Vec4f resultRow2 = (leftRow12 * rightColumn1) + (leftRow22 * rightColumn2) + (leftRow32 * rightColumn3);

                Vec4f leftRow13{M31};
                Vec4f leftRow23{M32};
                Vec4f leftRow33{M33};

                Vec4f resultRow3 = (leftRow13 * rightColumn1) + (leftRow23 * rightColumn2) + (leftRow33 * rightColumn3);

                Matrix3x3T result;

                resultRow1.store_a(&result.M11);
                resultRow2.store_a(&result.M21);
                resultRow3.store_a(&result.M31);

                return result;
            }
            else
            {
                return {
                    M11 * right.M11 + M12 * right.M21 + M13 * right.M31,
                    M11 * right.M12 + M12 * right.M22 + M13 * right.M32,
                    M11 * right.M13 + M12 * right.M23 + M13 * right.M33,

                    M21 * right.M11 + M22 * right.M21 + M23 * right.M31,
                    M21 * right.M12 + M22 * right.M22 + M23 * right.M32,
                    M21 * right.M13 + M22 * right.M23 + M23 * right.M33,

                    M31 * right.M11 + M32 * right.M21 + M33 * right.M31,
                    M31 * right.M12 + M32 * right.M22 + M33 * right.M32,
                    M31 * right.M13 + M32 * right.M23 + M33 * right.M33,
                };
            }
        }

        inline constexpr Matrix3x3T operator+(T right) const
        {
            return {
                M11 + right,
                M12 + right,
                M13 + right,

                M21 + right,
                M22 + right,
                M23 + right,

                M31 + right,
                M32 + right,
                M33 + right,
            };
        }

        inline constexpr Matrix3x3T operator-(T right) const
        {
            return {
                M11 - right,
                M12 - right,
                M13 - right,

                M21 - right,
                M22 - right,
                M23 - right,

                M31 - right,
                M32 - right,
                M33 - right,
            };
        }

        inline constexpr Matrix3x3T operator*(T right) const
        {
            return {
                M11 * right,
                M12 * right,
                M13 * right,

                M21 * right,
                M22 * right,
                M23 * right,

                M31 * right,
                M32 * right,
                M33 * right,
            };
        }

        inline constexpr Matrix3x3T operator/(T right) const
        {
            return {
                M11 / right,
                M12 / right,
                M13 / right,

                M21 / right,
                M22 / right,
                M23 / right,

                M31 / right,
                M32 / right,
                M33 / right,
            };
        }

        inline constexpr Matrix3x3T& operator+=(const Matrix3x3T& other)
        {
            M11 += other.M11;
            M12 += other.M12;
            M13 += other.M13;

            M21 += other.M21;
            M22 += other.M22;
            M23 += other.M23;

            M31 += other.M31;
            M32 += other.M32;
            M33 += other.M33;

            return *this;
        }

        inline constexpr Matrix3x3T& operator-=(const Matrix3x3T& other)
        {
            M11 -= other.M11;
            M12 -= other.M12;
            M13 -= other.M13;

            M21 -= other.M21;
            M22 -= other.M22;
            M23 -= other.M23;

            M31 -= other.M31;
            M32 -= other.M32;
            M33 -= other.M33;

            return *this;
        }

        inline constexpr Matrix3x3T& operator*=(const Matrix3x3T& other)
        {
            *this = *this * other;
            return *this;
        }

        inline constexpr Matrix3x3T& operator+=(T other)
        {
            M11 += other;
            M12 += other;
            M13 += other;

            M21 += other;
            M22 += other;
            M23 += other;

            M31 += other;
            M32 += other;
            M33 += other;

            return *this;
        }

        inline constexpr Matrix3x3T& operator-=(T other)
        {
            M11 -= other;
            M12 -= other;
            M13 -= other;

            M21 -= other;
            M22 -= other;
            M23 -= other;

            M31 -= other;
            M32 -= other;
            M33 -= other;

            return *this;
        }

        inline constexpr Matrix3x3T& operator*=(T other)
        {
            M11 *= other;
            M12 *= other;
            M13 *= other;

            M21 *= other;
            M22 *= other;
            M23 *= other;

            M31 *= other;
            M32 *= other;
            M33 *= other;

            return *this;
        }

        inline constexpr Matrix3x3T& operator/=(T other)
        {
            M11 /= other;
            M12 /= other;
            M13 /= other;

            M21 /= other;
            M22 /= other;
            M23 /= other;

            M31 /= other;
            M32 /= other;
            M33 /= other;

            return *this;
        }
    };

    export template <real_number T>
    inline constexpr Matrix3x3T<T> operator*(T left, const Matrix3x3T<T>& right)
    {
        return {
            left * right.M11,
            left * right.M12,
            left * right.M13,

            left * right.M21,
            left * right.M22,
            left * right.M23,

            left * right.M31,
            left * right.M32,
            left * right.M33,
        };
    }

    export template <real_number T>
    inline constexpr Vector3T<T> operator*(const Vector3T<T>& left, const Matrix3x3T<T>& right)
    {
        return {
            (left.X * right.M11) + (left.Y * right.M21) + (left.Z * right.M31),
            (left.X * right.M12) + (left.Y * right.M22) + (left.Z * right.M32),
            (left.X * right.M13) + (left.Y * right.M23) + (left.Z * right.M33),
        };
    }

    export using Matrix3x3 = Matrix3x3T<real>;
    export using FloatMatrix3x3 = Matrix3x3T<float>;
    export using DoubleMatrix3x3 = Matrix3x3T<double>;
}