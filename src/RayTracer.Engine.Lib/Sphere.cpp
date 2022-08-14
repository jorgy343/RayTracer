#include "Sphere.h"

using namespace RayTracer;

Sphere::Sphere()
    : Position(0.0f, 0.0f, 0.0f), _padding(0.0f), Radius(0.0f)
{

}

Sphere::Sphere(const Vector3& position, float radius)
	: Position(position), _padding(0.0f), Radius(radius)
{

}

float RayTracer::Sphere::Intersect(const Ray& ray) const
{
    auto v = ray.Position - Position;

    auto a = ray.Direction.Dot(ray.Direction);
    auto b = v.Dot(ray.Direction);
    auto c = v.Dot(v) - (Radius * Radius);

    auto discriminant = (b * b) - (a * c);
    if (discriminant < 0.0f)
    {
        return NAN;
    }

    auto discriminantSqrt = sqrtf(discriminant);
    auto inverseA = 1.0f / a;
    auto negativeB = -b;

    auto exitDistance = (negativeB + discriminantSqrt) * inverseA;
    if (exitDistance < 0.0f)
    {
        return NAN;
    }

    auto entranceDistance = (negativeB - discriminantSqrt) * inverseA;
    return fmaxf(entranceDistance, 0.0f);
}