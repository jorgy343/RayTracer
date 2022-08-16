#include "pch.h"

#include "Sphere.h"

using namespace RayTracer;

TEST(SphereIntersectionTests, RayOutside_Intersects_ReturnsNearestIntersection)
{
	// Arrange
	auto sphere = Sphere(Vector3(10, 0, 0), 2);
	auto ray = Ray(Vector3(2, 0, 0), Vector3(1, 0, 0));

	// Act
	auto result = sphere.Intersect(ray);

	// Assert
	EXPECT_NEAR(result, 6, 0.01f);
}