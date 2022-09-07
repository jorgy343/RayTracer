#include "pch.h"

#include <limits>

import RayTracer.Sphere;

using namespace RayTracer;

TEST(SphereIntersectionTests, RayOutsideGeometry_Intersects_ReturnsNearestIntersection)
{
    // Arrange
    Sphere sphere{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    // Act
    float result = sphere.IntersectEntrance(ray);

    // Assert
    EXPECT_NEAR(result, 6, 0.01f);
}

TEST(SphereIntersectionTests, RayOutsideGeometryAndPointingBackwards_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {-1, 0, 0}};

    // Act
    float result = sphere.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result, std::numeric_limits<float>::infinity());
}

TEST(SphereIntersectionTests, RayOutsideGeometry_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {0, 1, 0}};

    // Act
    float result = sphere.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result, std::numeric_limits<float>::infinity());
}

TEST(SphereIntersectionTests, RayInsideGeometry_Intersects_ReturnsZero)
{
    // Arrange
    Sphere sphere{{2, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    // Act
    float result = sphere.IntersectEntrance(ray);

    // Assert
    EXPECT_NEAR(result, 0, 0.01f);
}

TEST(SphereIntersectionTests, InfiniteSphere_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere{{std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()}, std::numeric_limits<float>::infinity(), nullptr};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    // Act
    float result = sphere.IntersectEntrance(ray);

    // Assert
    EXPECT_EQ(result, std::numeric_limits<float>::infinity());
}