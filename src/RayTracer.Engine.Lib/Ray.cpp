#include "Ray.h"

using namespace RayTracer;

Ray::Ray()
	: Position(0.0f, 0.0f, 0.0f), Direction(0.0f, 0.0f, 0.0f)
{

}

Ray::Ray(const Vector3& position, const Vector3& direction)
	: Position(position), Direction(direction)
{

}