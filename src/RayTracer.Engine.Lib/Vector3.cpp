#include "Vector3.h"

using namespace RayTracer;

Vector3::Vector3()
	: X(0.0f), Y(0.0f), Z(0.0f)
{

}

Vector3::Vector3(float scalar)
	: X(scalar), Y(scalar), Z(scalar)
{

}

Vector3::Vector3(float x, float y, float z)
	: X(x), Y(y), Z(z)
{

}

Vector3& Vector3::Normalize()
{
	auto inverseLength = 1.0f / Length();

	X *= inverseLength;
	Y *= inverseLength;
	Z *= inverseLength;

	return *this;
}

float Vector3::Dot(const Vector3& right) const
{
	return (X * right.X) + (Y * right.Y) + (Z * right.Z);
}

Vector3 Vector3::CrossProduct(const Vector3& right) const
{
	return Vector3(
		Y * right.Z - Z * right.Y,
		Z * right.X - X * right.Z,
		X * right.Y - Y * right.X);
}

float Vector3::Length() const
{
	return sqrtf(LengthSquared());
}

float Vector3::LengthSquared() const
{
	return (X * X) + (Y * Y) + (Z * Z);
}

Vector3& Vector3::operator+()
{
	return *this;
}

Vector3& Vector3::operator-()
{
	X = -X;
	Y = -Y;
	Z = -Z;

	return *this;
}

Vector3 Vector3::operator+(const Vector3& right) const
{
	return Vector3(X + right.X, Y + right.Y, Z + right.Z);
}

Vector3 Vector3::operator-(const Vector3& right) const
{
	return Vector3(X - right.X, Y - right.Y, Z - right.Z);
}

Vector3 Vector3::operator*(const Vector3& right) const
{
	return Vector3(X * right.X, Y * right.Y, Z * right.Z);
}

Vector3 Vector3::operator/(const Vector3& right) const
{
	return Vector3(X / right.X, Y / right.Y, Z / right.Z);
}

Vector3 Vector3::operator+(float right) const
{
	return Vector3(X + right, Y + right, Z + right);
}

Vector3 Vector3::operator-(float right) const
{
	return Vector3(X - right, Y - right, Z - right);
}

Vector3 Vector3::operator*(float right) const
{
	return Vector3(X * right, Y * right, Z * right);
}

Vector3 Vector3::operator/(float right) const
{
	return Vector3(X / right, Y / right, Z / right);
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;

	return *this;
}

Vector3& Vector3::operator*=(const Vector3& other)
{
	X *= other.X;
	Y *= other.Y;
	Z *= other.Z;

	return *this;
}

Vector3& Vector3::operator/=(const Vector3& other)
{
	X /= other.X;
	Y /= other.Y;
	Z /= other.Z;

	return *this;
}

Vector3& Vector3::operator+=(float other)
{
	X += other;
	Y += other;
	Z += other;

	return *this;
}

Vector3& Vector3::operator-=(float other)
{
	X -= other;
	Y -= other;
	Z -= other;

	return *this;
}

Vector3& Vector3::operator*=(float other)
{
	X *= other;
	Y *= other;
	Z *= other;

	return *this;
}

Vector3& Vector3::operator/=(float other)
{
	X /= other;
	Y /= other;
	Z /= other;

	return *this;
}

Vector3 RayTracer::operator+(float left, const Vector3& right)
{
	return Vector3(left + right.X, left + right.Y, left + right.Z);
}

Vector3 RayTracer::operator*(float left, const Vector3& right)
{
	return Vector3(left * right.X, left * right.Y, left * right.Z);
}