#include <cmath>

#include "Vcl.h"

export module RayTracing.Matrix3x3;

import RayTracer.Simd;
import RayTracer.Vector3;

using namespace vcl;

namespace RayTracer
{
    export class alignas(64) Matrix3x3
    {
    public:
        float M11{0.0f}, M12{0.0f}, M13{0.0f};
    private:
        float __DummyM14{0.0f};
    public:
        float M21{0.0f}, M22{0.0f}, M23{0.0f};
    private:
        float __DummyM24{0.0f};
    public:
        float M31{0.0f}, M32{0.0f}, M33{0.0f};
    private:
        float __DummyM34{0.0f};

    public:

        Matrix3x3() = default;

        explicit Matrix3x3(float scalar)
            :
            M11{scalar}, M12{scalar}, M13{scalar},
            M21{scalar}, M22{scalar}, M23{scalar},
            M31{scalar}, M32{scalar}, M33{scalar}
        {

        }

        Matrix3x3(
            float m11, float m12, float m13,
            float m21, float m22, float m23,
            float m31, float m32, float m33)
            :
            M11{m11}, M12{m12}, M13{m13},
            M21{m21}, M22{m22}, M23{m23},
            M31{m31}, M32{m32}, M33{m33}
        {

        }

        Matrix3x3(
            const Vector3& row1,
            const Vector3& row2,
            const Vector3& row3)
            :
            M11{row1.X}, M12{row1.Y}, M13{row1.Z},
            M21{row2.X}, M22{row2.Y}, M23{row2.Z},
            M31{row3.X}, M32{row3.Y}, M33{row3.Z}
        {

        }

        float Determinant() const
        {
            return
                (M11 * M22 * M33) +
                (M12 * M23 * M13) +
                (M13 * M21 * M32) -
                (M13 * M22 * M32) -
                (M12 * M21 * M33) -
                (M11 * M23 * M32);
        }

        Matrix3x3& Invert()
        {
            return *this;
        }

        inline Matrix3x3& Transpose()
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

        Matrix3x3 operator+() const
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

        Matrix3x3 operator-() const
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

        Matrix3x3& operator++()
        {
            M11 += 1.0f;
            M12 += 1.0f;
            M13 += 1.0f;

            M21 += 1.0f;
            M22 += 1.0f;
            M23 += 1.0f;

            M31 += 1.0f;
            M32 += 1.0f;
            M33 += 1.0f;

            return *this;
        }

        Matrix3x3& operator--()
        {
            M11 -= 1.0f;
            M12 -= 1.0f;
            M13 -= 1.0f;

            M21 -= 1.0f;
            M22 -= 1.0f;
            M23 -= 1.0f;

            M31 -= 1.0f;
            M32 -= 1.0f;
            M33 -= 1.0f;

            return *this;
        }

        Matrix3x3 operator++(int)
        {
            Matrix3x3 temp = *this;

            M11 += 1.0f;
            M12 += 1.0f;
            M13 += 1.0f;

            M21 += 1.0f;
            M22 += 1.0f;
            M23 += 1.0f;

            M31 += 1.0f;
            M32 += 1.0f;
            M33 += 1.0f;

            return temp;
        }

        Matrix3x3 operator--(int)
        {
            Matrix3x3 temp = *this;

            M11 -= 1.0f;
            M12 -= 1.0f;
            M13 -= 1.0f;

            M21 -= 1.0f;
            M22 -= 1.0f;
            M23 -= 1.0f;

            M31 -= 1.0f;
            M32 -= 1.0f;
            M33 -= 1.0f;

            return temp;
        }

        Matrix3x3 operator+(const Matrix3x3& right) const
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

        Matrix3x3 operator-(const Matrix3x3& right) const
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

        Matrix3x3 operator*(const Matrix3x3& right) const
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

            Matrix3x3 result;

            resultRow1.store_a(&result.M11);
            resultRow2.store_a(&result.M21);
            resultRow3.store_a(&result.M31);

            return result;
        }

        Matrix3x3 operator+(float right) const
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

        Matrix3x3 operator-(float right) const
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

        Matrix3x3 operator*(float right) const
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

        Matrix3x3 operator/(float right) const
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

        Matrix3x3& operator+=(const Matrix3x3& other)
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

        Matrix3x3& operator-=(const Matrix3x3& other)
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

        Matrix3x3& operator+=(float other)
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

        Matrix3x3& operator-=(float other)
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

        Matrix3x3& operator*=(float other)
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

        Matrix3x3& operator/=(float other)
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

    export Matrix3x3 operator*(float left, const Matrix3x3& right)
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

    export Vector3 operator*(const Vector3& left, const Matrix3x3& right)
    {
        return {
            (left.X * right.M11) + (left.Y * right.M21) + (left.Z * right.M31),
            (left.X * right.M12) + (left.Y * right.M22) + (left.Z * right.M32),
            (left.X * right.M13) + (left.Y * right.M23) + (left.Z * right.M33),
        };
    }
}