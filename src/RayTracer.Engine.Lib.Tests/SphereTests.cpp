#include "pch.h"

#include <limits>

import RayTracer.Sphere;

using namespace RayTracer;

TEST(SphereIntersectionTests, RayOutsideShape_Intersects_ReturnsNearestIntersection)
{
    // Arrange
    Sphere sphere{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    // Act
    float result = sphere.Intersect(ray);

    // Assert
    EXPECT_NEAR(result, 6, 0.01f);
}

TEST(SphereIntersectionTests, RayOutsideShapeAndPointingBackwards_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {-1, 0, 0}};

    // Act
    float result = sphere.Intersect(ray);

    // Assert
    EXPECT_EQ(result, std::numeric_limits<float>::infinity());
}

TEST(SphereIntersectionTests, RayOutsideShape_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere{{10, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {0, 1, 0}};

    // Act
    float result = sphere.Intersect(ray);

    // Assert
    EXPECT_EQ(result, std::numeric_limits<float>::infinity());
}

TEST(SphereIntersectionTests, RayInsideShape_Intersects_ReturnsZero)
{
    // Arrange
    Sphere sphere{{2, 0, 0}, 2, nullptr};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    // Act
    float result = sphere.Intersect(ray);

    // Assert
    EXPECT_NEAR(result, 0, 0.01f);
}