#include "pch.h"

import IntersectionResult;
import Sphere;
import SphereSoa;
import Vector3;

using namespace RayTracer;

TEST(SphereSoaIntersectionTests, OneSphere_RayOutsideGeometry_Intersects_ReturnsNearestIntersection)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    SphereSoa sphereSoa{};
    sphereSoa.Insert(0, &sphere1);

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_NEAR(result.HitDistance, 6, 0.01f);
    EXPECT_EQ(result.HitGeometry, &sphere1);
}

TEST(SphereSoaIntersectionTests, OneSphere_RayOutsideGeometryAndPointingBackwards_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {-1, 0, 0}};

    SphereSoa sphereSoa{};
    sphereSoa.Insert(0, &sphere1);

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result.HitDistance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.HitGeometry, nullptr);
}

TEST(SphereSoaIntersectionTests, OneSphere_RayOutsideGeometry_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {0, 1, 0}};

    SphereSoa sphereSoa{};
    sphereSoa.Insert(0, &sphere1);

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result.HitDistance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.HitGeometry, nullptr);
}

TEST(SphereSoaIntersectionTests, OneSphere_RayInsideGeometry_Intersects_ReturnsZero)
{
    // Arrange
    Sphere sphere1{{2, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    SphereSoa sphereSoa{};
    sphereSoa.Insert(0, &sphere1);

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_NEAR(result.HitDistance, 0, 0.01f);
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

    int i = 0;

    sphereSoa.Insert(i++, &sphere2);
    sphereSoa.Insert(i++, &sphere3);
    sphereSoa.Insert(i++, &sphere4);
    sphereSoa.Insert(i++, &sphere5);
    sphereSoa.Insert(i++, &sphere6);
    sphereSoa.Insert(i++, &sphere1); // This is the sphere that will be hit.
    sphereSoa.Insert(i++, &sphere7);
    sphereSoa.Insert(i++, &sphere8);

    Ray ray{{2, 0, 0}, {1, 0, 0}};

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_NEAR(result.HitDistance, 6, 0.01f);
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

    int i = 0;

    sphereSoa.Insert(i++, &sphere2);
    sphereSoa.Insert(i++, &sphere3);
    sphereSoa.Insert(i++, &sphere4);
    sphereSoa.Insert(i++, &sphere5);
    sphereSoa.Insert(i++, &sphere6);
    sphereSoa.Insert(i++, &sphere1); // This is the sphere that will be hit.
    sphereSoa.Insert(i++, &sphere7);
    sphereSoa.Insert(i++, &sphere8);

    Ray ray{{2, 0, 0}, {-1, 0, 0}};

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result.HitDistance, std::numeric_limits<float>::infinity());
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

    int i = 0;

    sphereSoa.Insert(i++, &sphere2);
    sphereSoa.Insert(i++, &sphere3);
    sphereSoa.Insert(i++, &sphere4);
    sphereSoa.Insert(i++, &sphere5);
    sphereSoa.Insert(i++, &sphere6);
    sphereSoa.Insert(i++, &sphere1); // This is the sphere that will be hit.
    sphereSoa.Insert(i++, &sphere7);
    sphereSoa.Insert(i++, &sphere8);

    Ray ray{{2, 0, 0}, {0, 1, 0}};

    // Act
    IntersectionResult result = sphereSoa.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result.HitDistance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.HitGeometry, nullptr);
}