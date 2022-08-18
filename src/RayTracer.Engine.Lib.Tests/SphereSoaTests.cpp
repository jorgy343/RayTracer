#include "pch.h"

#include "SphereSoa.h"

using namespace RayTracer;

TEST(SphereSoaIntersectionTests, RayOutside_Intersects_ReturnsNearestIntersection)
{
	// Arrange
	auto sphere1 = Sphere(Vector3(10, 0, 0), 2);
	auto sphere2 = Sphere(Vector3(12, 0, 0), 2);
	auto sphere3 = Sphere(Vector3(14, 0, 0), 2);
	auto sphere4 = Sphere(Vector3(16, 0, 0), 2);
	auto sphere5 = Sphere(Vector3(18, 0, 0), 2);
	auto sphere6 = Sphere(Vector3(20, 0, 0), 2);
	auto sphere7 = Sphere(Vector3(22, 0, 0), 2);
	auto sphere8 = Sphere(Vector3(24, 0, 0), 2);

	auto sphereSoa = SphereSoa();

	sphereSoa.AddSphere(&sphere2);
	sphereSoa.AddSphere(&sphere3);
	sphereSoa.AddSphere(&sphere4);
	sphereSoa.AddSphere(&sphere5);
	sphereSoa.AddSphere(&sphere6);
	sphereSoa.AddSphere(&sphere1); // This is the sphere that will be hit.
	sphereSoa.AddSphere(&sphere7);
	sphereSoa.AddSphere(&sphere8);

	auto ray = Ray(Vector3(2, 0, 0), Vector3(1, 0, 0));

	// Act
	auto result = sphereSoa.Intersect(ray);

	// Assert
	EXPECT_NEAR(result.Distance, 6, 0.01f);
	EXPECT_EQ(result.Sphere, &sphere1);
}

TEST(SphereSoaIntersectionSimdTests, RayOutside_Intersects_ReturnsNearestIntersection)
{
	// Arrange
	auto sphere1 = Sphere(Vector3(10, 0, 0), 2);
	auto sphere2 = Sphere(Vector3(12, 0, 0), 2);
	auto sphere3 = Sphere(Vector3(14, 0, 0), 2);
	auto sphere4 = Sphere(Vector3(16, 0, 0), 2);
	auto sphere5 = Sphere(Vector3(18, 0, 0), 2);
	auto sphere6 = Sphere(Vector3(20, 0, 0), 2);
	auto sphere7 = Sphere(Vector3(22, 0, 0), 2);
	auto sphere8 = Sphere(Vector3(24, 0, 0), 2);

	auto sphereSoa = SphereSoa();

	sphereSoa.AddSphere(&sphere2);
	sphereSoa.AddSphere(&sphere3);
	sphereSoa.AddSphere(&sphere4);
	sphereSoa.AddSphere(&sphere5);
	sphereSoa.AddSphere(&sphere6);
	sphereSoa.AddSphere(&sphere1); // This is the sphere that will be hit.
	sphereSoa.AddSphere(&sphere7);
	sphereSoa.AddSphere(&sphere8);

	auto ray = Ray(Vector3(2, 0, 0), Vector3(1, 0, 0));

	// Act
	auto result = sphereSoa.IntersectSimd(ray);

	// Assert
	EXPECT_NEAR(result.Distance, 6, 0.01f);
	EXPECT_EQ(result.Sphere, &sphere1);
}

TEST(SphereSoaIntersectionSimdTests, RayFacingBackwards_ReturnsNan)
{
	// Arrange
	auto sphere1 = Sphere(Vector3(10, 0, 0), 2);
	auto sphere2 = Sphere(Vector3(12, 0, 0), 2);
	auto sphere3 = Sphere(Vector3(14, 0, 0), 2);
	auto sphere4 = Sphere(Vector3(16, 0, 0), 2);
	auto sphere5 = Sphere(Vector3(18, 0, 0), 2);
	auto sphere6 = Sphere(Vector3(20, 0, 0), 2);
	auto sphere7 = Sphere(Vector3(22, 0, 0), 2);
	auto sphere8 = Sphere(Vector3(24, 0, 0), 2);

	auto sphereSoa = SphereSoa();

	sphereSoa.AddSphere(&sphere2);
	sphereSoa.AddSphere(&sphere3);
	sphereSoa.AddSphere(&sphere4);
	sphereSoa.AddSphere(&sphere5);
	sphereSoa.AddSphere(&sphere6);
	sphereSoa.AddSphere(&sphere1);
	sphereSoa.AddSphere(&sphere7);
	sphereSoa.AddSphere(&sphere8);

	auto ray = Ray(Vector3(2, 0, 0), Vector3(-1, 0, 0)); // Ray is pointing backwards so it should hit spheres but the results will be thrown out.

	// Act
	auto result = sphereSoa.IntersectSimd(ray);

	// Assert
	EXPECT_TRUE(std::isnan(result.Distance));
	EXPECT_EQ(result.Sphere, &sphere8);
}

TEST(SphereSoaIntersectionSimdTests, RayMissesAllSpheres_ReturnsNan)
{
	// Arrange
	auto sphere1 = Sphere(Vector3(10, 0, 0), 2);
	auto sphere2 = Sphere(Vector3(12, 0, 0), 2);
	auto sphere3 = Sphere(Vector3(14, 0, 0), 2);
	auto sphere4 = Sphere(Vector3(16, 0, 0), 2);
	auto sphere5 = Sphere(Vector3(18, 0, 0), 2);
	auto sphere6 = Sphere(Vector3(20, 0, 0), 2);
	auto sphere7 = Sphere(Vector3(22, 0, 0), 2);
	auto sphere8 = Sphere(Vector3(24, 0, 0), 2);

	auto sphereSoa = SphereSoa();

	sphereSoa.AddSphere(&sphere2);
	sphereSoa.AddSphere(&sphere3);
	sphereSoa.AddSphere(&sphere4);
	sphereSoa.AddSphere(&sphere5);
	sphereSoa.AddSphere(&sphere6);
	sphereSoa.AddSphere(&sphere1);
	sphereSoa.AddSphere(&sphere7);
	sphereSoa.AddSphere(&sphere8);

	auto ray = Ray(Vector3(2, 0, 0), Vector3(0, 1, 0)); // Ray pointing up instead of forward, will miss all spheres.

	// Act
	auto result = sphereSoa.IntersectSimd(ray);

	// Assert
	EXPECT_TRUE(std::isnan(result.Distance));
	EXPECT_EQ(result.Sphere, &sphere8);
}