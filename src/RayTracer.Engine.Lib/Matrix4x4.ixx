#include <cmath>

#include "Vcl.h"

export module RayTracer.Matrix4x4;

import RayTracer.Math;
import RayTracer.Vector4;

using namespace vcl;

namespace RayTracer
{
    export class alignas(64) Matrix4x4
    {
    public:
        float
            M11{0.0f}, M12{0.0f}, M13{0.0f}, M14{0.0f},
            M21{0.0f}, M22{0.0f}, M23{0.0f}, M24{0.0f},
            M31{0.0f}, M32{0.0f}, M33{0.0f}, M34{0.0f},
            M41{0.0f}, M42{0.0f}, M43{0.0f}, M44{0.0f};

        Matrix4x4() = default;

        explicit Matrix4x4(float scalar)
            :
            M11{scalar}, M12{scalar}, M13{scalar}, M14{scalar},
            M21{scalar}, M22{scalar}, M23{scalar}, M24{scalar},
            M31{scalar}, M32{scalar}, M33{scalar}, M34{scalar},
            M41{scalar}, M42{scalar}, M43{scalar}, M44{scalar}
        {

        }

        Matrix4x4(
            float m11, float m12, float m13, float m14,
            float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44)
            :
            M11{m11}, M12{m12}, M13{m13}, M14{m14},
            M21{m21}, M22{m22}, M23{m23}, M24{m24},
            M31{m31}, M32{m32}, M33{m33}, M34{m34},
            M41{m41}, M42{m42}, M43{m43}, M44{m44}
        {

        }

        Matrix4x4(
            const Vector4& row1,
            const Vector4& row2,
            const Vector4& row3,
            const Vector4& row4)
            :
            M11{row1.X}, M12{row1.Y}, M13{row1.Z}, M14{row1.Z},
            M21{row2.X}, M22{row2.Y}, M23{row2.Z}, M24{row2.Z},
            M31{row3.X}, M32{row3.Y}, M33{row3.Z}, M34{row3.Z},
            M41{row4.X}, M42{row4.Y}, M43{row4.Z}, M44{row4.Z}
        {

        }

        float Determinant() const
        {
            return
                (M14 * M23 * M32 * M41) - (M13 * M24 * M32 * M41) - (M14 * M22 * M33 * M41) + (M12 * M24 * M33 * M41) +
                (M13 * M22 * M34 * M41) - (M12 * M23 * M34 * M41) - (M14 * M23 * M31 * M42) + (M13 * M24 * M31 * M42) +
                (M14 * M21 * M33 * M42) - (M11 * M24 * M33 * M42) - (M13 * M21 * M34 * M42) + (M11 * M23 * M34 * M42) +
                (M14 * M22 * M31 * M43) - (M12 * M24 * M31 * M43) - (M14 * M21 * M32 * M43) + (M11 * M24 * M32 * M43) +
                (M12 * M21 * M34 * M43) - (M11 * M22 * M34 * M43) - (M13 * M22 * M31 * M44) + (M12 * M23 * M31 * M44) +
                (M13 * M21 * M32 * M44) - (M11 * M23 * M32 * M44) - (M12 * M21 * M33 * M44) + (M11 * M22 * M33 * M44);
        }

        Matrix4x4& Invert()
        {
            float b0 = (M31 * M42) - (M32 * M41);
            float b1 = (M31 * M43) - (M33 * M41);
            float b2 = (M34 * M41) - (M31 * M44);
            float b3 = (M32 * M43) - (M33 * M42);
            float b4 = (M34 * M42) - (M32 * M44);
            float b5 = (M33 * M44) - (M34 * M43);

            float d11 = M22 * b5 + M23 * b4 + M24 * b3;
            float d12 = M21 * b5 + M23 * b2 + M24 * b1;
            float d13 = M21 * -b4 + M22 * b2 + M24 * b0;
            float d14 = M21 * b3 + M22 * -b1 + M23 * b0;

            float det = M11 * d11 - M12 * d12 + M13 * d13 - M14 * d14;

            // Normally you would check if the determinant is zero here and handle that case.
            // We're just going to assume it is not zero and continue as normal.

            det = FastReciprical(det);

            float a0 = (M11 * M22) - (M12 * M21);
            float a1 = (M11 * M23) - (M13 * M21);
            float a2 = (M14 * M21) - (M11 * M24);
            float a3 = (M12 * M23) - (M13 * M22);
            float a4 = (M14 * M22) - (M12 * M24);
            float a5 = (M13 * M24) - (M14 * M23);

            float d21 = M12 * b5 + M13 * b4 + M14 * b3;
            float d22 = M11 * b5 + M13 * b2 + M14 * b1;
            float d23 = M11 * -b4 + M12 * b2 + M14 * b0;
            float d24 = M11 * b3 + M12 * -b1 + M13 * b0;

            float d31 = M42 * a5 + M43 * a4 + M44 * a3;
            float d32 = M41 * a5 + M43 * a2 + M44 * a1;
            float d33 = M41 * -a4 + M42 * a2 + M44 * a0;
            float d34 = M41 * a3 + M42 * -a1 + M43 * a0;

            float d41 = M32 * a5 + M33 * a4 + M34 * a3;
            float d42 = M31 * a5 + M33 * a2 + M34 * a1;
            float d43 = M31 * -a4 + M32 * a2 + M34 * a0;
            float d44 = M31 * a3 + M32 * -a1 + M33 * a0;

            M11 = +d11 * det;
            M12 = -d21 * det;
            M13 = +d31 * det;
            M14 = -d41 * det;

            M21 = -d12 * det;
            M22 = +d22 * det;
            M23 = -d32 * det;
            M24 = +d42 * det;

            M31 = +d13 * det;
            M32 = -d23 * det;
            M33 = +d33 * det;
            M34 = -d43 * det;

            M41 = -d14 * det;
            M42 = +d24 * det;
            M43 = -d34 * det;
            M44 = +d44 * det;

            return *this;
        }

        inline Matrix4x4& Transpose()
        {
            M11 = M11;
            M12 = M21;
            M13 = M31;
            M14 = M41;

            M21 = M12;
            M22 = M22;
            M23 = M32;
            M24 = M42;

            M31 = M13;
            M32 = M23;
            M33 = M33;
            M34 = M43;

            M41 = M14;
            M42 = M24;
            M43 = M34;
            M44 = M44;

            return *this;
        }

        Matrix4x4 operator+() const
        {
            return {
                +M11,
                +M12,
                +M13,
                +M14,

                +M21,
                +M22,
                +M23,
                +M24,

                +M31,
                +M32,
                +M33,
                +M34,

                +M41,
                +M42,
                +M43,
                +M44,
            };
        }

        Matrix4x4 operator-() const
        {
            return {
                -M11,
                -M12,
                -M13,
                -M14,

                -M21,
                -M22,
                -M23,
                -M24,

                -M31,
                -M32,
                -M33,
                -M34,

                -M41,
                -M42,
                -M43,
                -M44,
            };
        }

        Matrix4x4& operator++()
        {
            M11 += 1.0f;
            M12 += 1.0f;
            M13 += 1.0f;
            M14 += 1.0f;

            M21 += 1.0f;
            M22 += 1.0f;
            M23 += 1.0f;
            M24 += 1.0f;

            M31 += 1.0f;
            M32 += 1.0f;
            M33 += 1.0f;
            M34 += 1.0f;

            M41 += 1.0f;
            M42 += 1.0f;
            M43 += 1.0f;
            M44 += 1.0f;

            return *this;
        }

        Matrix4x4& operator--()
        {
            M11 -= 1.0f;
            M12 -= 1.0f;
            M13 -= 1.0f;
            M14 -= 1.0f;

            M21 -= 1.0f;
            M22 -= 1.0f;
            M23 -= 1.0f;
            M24 -= 1.0f;

            M31 -= 1.0f;
            M32 -= 1.0f;
            M33 -= 1.0f;
            M34 -= 1.0f;

            M41 -= 1.0f;
            M42 -= 1.0f;
            M43 -= 1.0f;
            M44 -= 1.0f;

            return *this;
        }

        Matrix4x4 operator++(int)
        {
            Matrix4x4 temp = *this;

            M11 += 1.0f;
            M12 += 1.0f;
            M13 += 1.0f;
            M14 += 1.0f;

            M21 += 1.0f;
            M22 += 1.0f;
            M23 += 1.0f;
            M24 += 1.0f;

            M31 += 1.0f;
            M32 += 1.0f;
            M33 += 1.0f;
            M34 += 1.0f;

            M41 += 1.0f;
            M42 += 1.0f;
            M43 += 1.0f;
            M44 += 1.0f;

            return temp;
        }

        Matrix4x4 operator--(int)
        {
            Matrix4x4 temp = *this;

            M11 -= 1.0f;
            M12 -= 1.0f;
            M13 -= 1.0f;
            M14 -= 1.0f;

            M21 -= 1.0f;
            M22 -= 1.0f;
            M23 -= 1.0f;
            M24 -= 1.0f;

            M31 -= 1.0f;
            M32 -= 1.0f;
            M33 -= 1.0f;
            M34 -= 1.0f;

            M41 -= 1.0f;
            M42 -= 1.0f;
            M43 -= 1.0f;
            M44 -= 1.0f;

            return temp;
        }

        Matrix4x4 operator+(const Matrix4x4& right) const
        {
            return {
                M11 + right.M11,
                M12 + right.M12,
                M13 + right.M13,
                M14 + right.M14,

                M21 + right.M21,
                M22 + right.M22,
                M23 + right.M23,
                M24 + right.M24,

                M31 + right.M31,
                M32 + right.M32,
                M33 + right.M33,
                M34 + right.M34,

                M41 + right.M41,
                M42 + right.M42,
                M43 + right.M43,
                M44 + right.M44,
            };
        }

        Matrix4x4 operator-(const Matrix4x4& right) const
        {
            return {
                M11 - right.M11,
                M12 - right.M12,
                M13 - right.M13,
                M14 - right.M14,

                M21 - right.M21,
                M22 - right.M22,
                M23 - right.M23,
                M24 - right.M24,

                M31 - right.M31,
                M32 - right.M32,
                M33 - right.M33,
                M34 - right.M34,

                M41 - right.M41,
                M42 - right.M42,
                M43 - right.M43,
                M44 - right.M44,
            };
        }

        Matrix4x4 operator*(const Matrix4x4& right) const
        {
            // Currently this is slower on zen 2 architecture.
            //Vec4f rightColumn1 = gather4f<0, 4, 8, 12>(&right.M11);
            //Vec4f rightColumn2 = gather4f<1, 5, 9, 13>(&right.M11);
            //Vec4f rightColumn3 = gather4f<2, 6, 10, 14>(&right.M11);
            //Vec4f rightColumn4 = gather4f<3, 7, 11, 15>(&right.M11);

            Vec4f rightColumn1{right.M11, right.M12, right.M13, right.M14};
            Vec4f rightColumn2{right.M21, right.M22, right.M23, right.M24};
            Vec4f rightColumn3{right.M31, right.M32, right.M33, right.M34};
            Vec4f rightColumn4{right.M41, right.M42, right.M43, right.M44};

            Vec4f leftRow11{M11};
            Vec4f leftRow21{M12};
            Vec4f leftRow31{M13};
            Vec4f leftRow41{M14};

            Vec4f resultRow1 = (leftRow11 * rightColumn1) + (leftRow21 * rightColumn2) + (leftRow31 * rightColumn3) + (leftRow41 * rightColumn4);

            Vec4f leftRow12{M21};
            Vec4f leftRow22{M22};
            Vec4f leftRow32{M23};
            Vec4f leftRow42{M24};

            Vec4f resultRow2 = (leftRow12 * rightColumn1) + (leftRow22 * rightColumn2) + (leftRow32 * rightColumn3) + (leftRow42 * rightColumn4);

            Vec4f leftRow13{M31};
            Vec4f leftRow23{M32};
            Vec4f leftRow33{M33};
            Vec4f leftRow43{M34};

            Vec4f resultRow3 = (leftRow13 * rightColumn1) + (leftRow23 * rightColumn2) + (leftRow33 * rightColumn3) + (leftRow43 * rightColumn4);

            Vec4f leftRow14{M41};
            Vec4f leftRow24{M42};
            Vec4f leftRow34{M43};
            Vec4f leftRow44{M44};

            Vec4f resultRow4 = (leftRow14 * rightColumn1) + (leftRow24 * rightColumn2) + (leftRow34 * rightColumn3) + (leftRow44 * rightColumn4);

            Matrix4x4 result;

            resultRow1.store_a(&result.M11);
            resultRow2.store_a(&result.M21);
            resultRow3.store_a(&result.M31);
            resultRow4.store_a(&result.M41);

            return result;
        }

        Matrix4x4 operator+(float right) const
        {
            return {
                M11 + right,
                M12 + right,
                M13 + right,
                M14 + right,

                M21 + right,
                M22 + right,
                M23 + right,
                M24 + right,

                M31 + right,
                M32 + right,
                M33 + right,
                M34 + right,

                M41 + right,
                M42 + right,
                M43 + right,
                M44 + right,
            };
        }

        Matrix4x4 operator-(float right) const
        {
            return {
                M11 - right,
                M12 - right,
                M13 - right,
                M14 - right,

                M21 - right,
                M22 - right,
                M23 - right,
                M24 - right,

                M31 - right,
                M32 - right,
                M33 - right,
                M34 - right,

                M41 - right,
                M42 - right,
                M43 - right,
                M44 - right,
            };
        }

        Matrix4x4 operator*(float right) const
        {
            return {
                M11 * right,
                M12 * right,
                M13 * right,
                M14 * right,

                M21 * right,
                M22 * right,
                M23 * right,
                M24 * right,

                M31 * right,
                M32 * right,
                M33 * right,
                M34 * right,

                M41 * right,
                M42 * right,
                M43 * right,
                M44 * right,
            };
        }

        Matrix4x4 operator/(float right) const
        {
            return {
                M11 / right,
                M12 / right,
                M13 / right,
                M14 / right,

                M21 / right,
                M22 / right,
                M23 / right,
                M24 / right,

                M31 / right,
                M32 / right,
                M33 / right,
                M34 / right,

                M41 / right,
                M42 / right,
                M43 / right,
                M44 / right,
            };
        }

        Matrix4x4& operator+=(const Matrix4x4& other)
        {
            M11 += other.M11;
            M12 += other.M12;
            M13 += other.M13;
            M14 += other.M14;

            M21 += other.M21;
            M22 += other.M22;
            M23 += other.M23;
            M24 += other.M24;

            M31 += other.M31;
            M32 += other.M32;
            M33 += other.M33;
            M34 += other.M34;

            M41 += other.M41;
            M42 += other.M42;
            M43 += other.M43;
            M44 += other.M44;

            return *this;
        }

        Matrix4x4& operator-=(const Matrix4x4& other)
        {
            M11 -= other.M11;
            M12 -= other.M12;
            M13 -= other.M13;
            M14 -= other.M14;

            M21 -= other.M21;
            M22 -= other.M22;
            M23 -= other.M23;
            M24 -= other.M24;

            M31 -= other.M31;
            M32 -= other.M32;
            M33 -= other.M33;
            M34 -= other.M34;

            M41 -= other.M41;
            M42 -= other.M42;
            M43 -= other.M43;
            M44 -= other.M44;

            return *this;
        }

        Matrix4x4& operator+=(float other)
        {
            M11 += other;
            M12 += other;
            M13 += other;
            M14 += other;

            M21 += other;
            M22 += other;
            M23 += other;
            M24 += other;

            M31 += other;
            M32 += other;
            M33 += other;
            M34 += other;

            M41 += other;
            M42 += other;
            M43 += other;
            M44 += other;

            return *this;
        }

        Matrix4x4& operator-=(float other)
        {
            M11 -= other;
            M12 -= other;
            M13 -= other;
            M14 -= other;

            M21 -= other;
            M22 -= other;
            M23 -= other;
            M24 -= other;

            M31 -= other;
            M32 -= other;
            M33 -= other;
            M34 -= other;

            M41 -= other;
            M42 -= other;
            M43 -= other;
            M44 -= other;

            return *this;
        }

        Matrix4x4& operator*=(float other)
        {
            M11 *= other;
            M12 *= other;
            M13 *= other;
            M14 *= other;

            M21 *= other;
            M22 *= other;
            M23 *= other;
            M24 *= other;

            M31 *= other;
            M32 *= other;
            M33 *= other;
            M34 *= other;

            M41 *= other;
            M42 *= other;
            M43 *= other;
            M44 *= other;

            return *this;
        }

        Matrix4x4& operator/=(float other)
        {
            M11 /= other;
            M12 /= other;
            M13 /= other;
            M14 /= other;

            M21 /= other;
            M22 /= other;
            M23 /= other;
            M24 /= other;

            M31 /= other;
            M32 /= other;
            M33 /= other;
            M34 /= other;

            M41 /= other;
            M42 /= other;
            M43 /= other;
            M44 /= other;

            return *this;
        }
    };

    export Matrix4x4 operator*(float left, const Matrix4x4& right)
    {
        return {
            left * right.M11,
            left * right.M12,
            left * right.M13,
            left * right.M14,

            left * right.M21,
            left * right.M22,
            left * right.M23,
            left * right.M24,

            left * right.M31,
            left * right.M32,
            left * right.M33,
            left * right.M34,

            left * right.M41,
            left * right.M42,
            left * right.M43,
            left * right.M44,
        };
    }

    export Vector4 operator*(const Vector4& left, const Matrix4x4& right)
    {
        return {
            (left.X * right.M11) + (left.Y * right.M21) + (left.Z * right.M31) + (left.W * right.M41),
            (left.X * right.M12) + (left.Y * right.M22) + (left.Z * right.M32) + (left.W * right.M42),
            (left.X * right.M13) + (left.Y * right.M23) + (left.Z * right.M33) + (left.W * right.M43),
            (left.X * right.M14) + (left.Y * right.M24) + (left.Z * right.M34) + (left.W * right.M44),
        };
    }
}