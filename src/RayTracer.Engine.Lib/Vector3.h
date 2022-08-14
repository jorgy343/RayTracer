#pragma once

#include <cmath>

namespace RayTracer
{
	class Vector3
	{
	public:
		float X;
		float Y;
		float Z;

		Vector3();
		Vector3(float scalar);
		Vector3(float x, float y, float z);

		void Normalize();
		float Dot(const Vector3& right) const;

		float Length() const;
		float LengthSquared() const;

		Vector3& operator+();
		Vector3& operator-();

		Vector3 operator+(const Vector3& right) const;
		Vector3 operator-(const Vector3& right) const;
		Vector3 operator*(const Vector3& right) const;
		Vector3 operator/(const Vector3& right) const;

		Vector3 operator+(float right) const;
		Vector3 operator-(float right) const;
		Vector3 operator*(float right) const;
		Vector3 operator/(float right) const;

		friend Vector3 operator+(float left, const Vector3& right);
		friend Vector3 operator*(float left, const Vector3& right);

		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator*=(const Vector3& other);
		Vector3& operator/=(const Vector3& other);

		Vector3& operator+=(float other);
		Vector3& operator-=(float other);
		Vector3& operator*=(float other);
		Vector3& operator/=(float other);
	};

	Vector3 operator+(float left, const Vector3& right);
	Vector3 operator*(float left, const Vector3& right);
}