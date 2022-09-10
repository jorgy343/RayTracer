#include "pch.h"

import RayTracer.IntersectionResult;
import RayTracer.Sphere;
import RayTracer.SphereSoa;
import RayTracer.Vector3;

using namespace RayTracer;

TEST(SphereSoaIntersectionTests, OneSphere_RayOutsideGeometry_Intersects_ReturnsNearestIntersection)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    SphereSoa sphereSoa{};

    sphereSoa.Add(&sphere1);
    sphereSoa.Finalize();

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_NEAR(result.Distance, 6, 0.01f);
    EXPECT_EQ(result.HitGeometry, &sphere1);
}

TEST(SphereSoaIntersectionTests, OneSphere_RayOutsideGeometryAndPointingBackwards_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {-1, 0, 0}};

    SphereSoa sphereSoa{};

    sphereSoa.Add(&sphere1);
    sphereSoa.Finalize();

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result.Distance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.HitGeometry, nullptr);
}

TEST(SphereSoaIntersectionTests, OneSphere_RayOutsideGeometry_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {0, 1, 0}};

    SphereSoa sphereSoa{};

    sphereSoa.Add(&sphere1);
    sphereSoa.Finalize();

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result.Distance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.HitGeometry, nullptr);
}

TEST(SphereSoaIntersectionTests, OneSphere_RayInsideGeometry_Intersects_ReturnsZero)
{
    // Arrange
    Sphere sphere1{{2, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    SphereSoa sphereSoa{};

    sphereSoa.Add(&sphere1);
    sphereSoa.Finalize();

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_NEAR(result.Distance, 0, 0.01f);
    EXPECT_EQ(result.HitGeometry, &sphere1);
}

TEST(SphereSoaIntersectionTests, EightSpheres_RayOutsideGeometrys_Intersects_ReturnsNearestIntersection)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Sphere sphere2{{12, 0, 0}, 2, nullptr};
    Sphere sphere3{{14, 0, 0}, 2, nullptr};
    Sphere sphere4{{16, 0, 0}, 2, nullptr};
    Sphere sphere5{{18, 0, 0}, 2, nullptr};
    Sphere sphere6{{20, 0, 0}, 2, nullptr};
    Sphere sphere7{{22, 0, 0}, 2, nullptr};
    Sphere sphere8{{24, 0, 0}, 2, nullptr};

    SphereSoa sphereSoa{};

    sphereSoa.Add(&sphere2);
    sphereSoa.Add(&sphere3);
    sphereSoa.Add(&sphere4);
    sphereSoa.Add(&sphere5);
    sphereSoa.Add(&sphere6);
    sphereSoa.Add(&sphere1); // This is the sphere that will be hit.
    sphereSoa.Add(&sphere7);
    sphereSoa.Add(&sphere8);

    sphereSoa.Finalize();

    Ray ray{{2, 0, 0}, {1, 0, 0}};

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_NEAR(result.Distance, 6, 0.01f);
    EXPECT_EQ(result.HitGeometry, &sphere1);
}

TEST(SphereSoaIntersectionTests, RayOutsideGeometrysAndPointingBackwards_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Sphere sphere2{{12, 0, 0}, 2, nullptr};
    Sphere sphere3{{14, 0, 0}, 2, nullptr};
    Sphere sphere4{{16, 0, 0}, 2, nullptr};
    Sphere sphere5{{18, 0, 0}, 2, nullptr};
    Sphere sphere6{{20, 0, 0}, 2, nullptr};
    Sphere sphere7{{22, 0, 0}, 2, nullptr};
    Sphere sphere8{{24, 0, 0}, 2, nullptr};

    SphereSoa sphereSoa{};

    sphereSoa.Add(&sphere2);
    sphereSoa.Add(&sphere3);
    sphereSoa.Add(&sphere4);
    sphereSoa.Add(&sphere5);
    sphereSoa.Add(&sphere6);
    sphereSoa.Add(&sphere1); // This is the sphere that will be hit.
    sphereSoa.Add(&sphere7);
    sphereSoa.Add(&sphere8);

    sphereSoa.Finalize();

    Ray ray{{2, 0, 0}, {-1, 0, 0}};

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result.Distance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.HitGeometry, nullptr);
}

TEST(SphereSoaIntersectionTests, RayOutsideGeometrys_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Sphere sphere2{{12, 0, 0}, 2, nullptr};
    Sphere sphere3{{14, 0, 0}, 2, nullptr};
    Sphere sphere4{{16, 0, 0}, 2, nullptr};
    Sphere sphere5{{18, 0, 0}, 2, nullptr};
    Sphere sphere6{{20, 0, 0}, 2, nullptr};
    Sphere sphere7{{22, 0, 0}, 2, nullptr};
    Sphere sphere8{{24, 0, 0}, 2, nullptr};

    SphereSoa sphereSoa{};

    sphereSoa.Add(&sphere2);
    sphereSoa.Add(&sphere3);
    sphereSoa.Add(&sphere4);
    sphereSoa.Add(&sphere5);
    sphereSoa.Add(&sphere6);
    sphereSoa.Add(&sphere1); // This is the sphere that will be hit.
    sphereSoa.Add(&sphere7);
    sphereSoa.Add(&sphere8);

    sphereSoa.Finalize();

    Ray ray{{2, 0, 0}, {0, 1, 0}};

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result.Distance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.HitGeometry, nullptr);
}