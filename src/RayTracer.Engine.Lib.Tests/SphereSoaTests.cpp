#include "pch.h"

import RayTracer.IntersectionResult;
import RayTracer.Sphere;
import RayTracer.SphereSoa;
import RayTracer.Vector3;

using namespace RayTracer;

TEST(SphereSoaIntersectionTests, OneSphere_RayOutsideShape_Intersects_ReturnsNearestIntersection)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    SphereSoa sphereSoa{};

    sphereSoa.AddSphere(&sphere1);
    sphereSoa.Finalize();

    // Act
    IntersectionResult<Sphere> result = sphereSoa.Intersect(ray);

    // Assert
    EXPECT_NEAR(result.Distance, 6, 0.01f);
    EXPECT_EQ(result.Shape, &sphere1);
}

TEST(SphereSoaIntersectionTests, OneSphere_RayOutsideShapeAndPointingBackwards_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2};
    Ray ray{{2, 0, 0}, {-1, 0, 0}};

    SphereSoa sphereSoa{};

    sphereSoa.AddSphere(&sphere1);
    sphereSoa.Finalize();

    // Act
    IntersectionResult<Sphere> result = sphereSoa.Intersect(ray);

    // Assert
    EXPECT_EQ(result.Distance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.Shape, nullptr);
}

TEST(SphereSoaIntersectionTests, OneSphere_RayOutsideShape_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2};
    Ray ray{{2, 0, 0}, {0, 1, 0}};

    SphereSoa sphereSoa{};

    sphereSoa.AddSphere(&sphere1);
    sphereSoa.Finalize();

    // Act
    IntersectionResult<Sphere> result = sphereSoa.Intersect(ray);

    // Assert
    EXPECT_EQ(result.Distance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.Shape, nullptr);
}

TEST(SphereSoaIntersectionTests, OneSphere_RayInsideShape_Intersects_ReturnsZero)
{
    // Arrange
    Sphere sphere1{{2, 0, 0}, 2};
    Ray ray{{2, 0, 0}, {1, 0, 0}};

    SphereSoa sphereSoa{};

    sphereSoa.AddSphere(&sphere1);
    sphereSoa.Finalize();

    // Act
    IntersectionResult<Sphere> result = sphereSoa.Intersect(ray);

    // Assert
    EXPECT_NEAR(result.Distance, 0, 0.01f);
    EXPECT_EQ(result.Shape, &sphere1);
}

TEST(SphereSoaIntersectionTests, EightSpheres_RayOutsideShapes_Intersects_ReturnsNearestIntersection)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2};
    Sphere sphere2{{12, 0, 0}, 2};
    Sphere sphere3{{14, 0, 0}, 2};
    Sphere sphere4{{16, 0, 0}, 2};
    Sphere sphere5{{18, 0, 0}, 2};
    Sphere sphere6{{20, 0, 0}, 2};
    Sphere sphere7{{22, 0, 0}, 2};
    Sphere sphere8{{24, 0, 0}, 2};

    SphereSoa sphereSoa{};

    sphereSoa.AddSphere(&sphere2);
    sphereSoa.AddSphere(&sphere3);
    sphereSoa.AddSphere(&sphere4);
    sphereSoa.AddSphere(&sphere5);
    sphereSoa.AddSphere(&sphere6);
    sphereSoa.AddSphere(&sphere1); // This is the sphere that will be hit.
    sphereSoa.AddSphere(&sphere7);
    sphereSoa.AddSphere(&sphere8);

    sphereSoa.Finalize();

    Ray ray{{2, 0, 0}, {1, 0, 0}};

    // Act
    IntersectionResult<Sphere> result = sphereSoa.Intersect(ray);

    // Assert
    EXPECT_NEAR(result.Distance, 6, 0.01f);
    EXPECT_EQ(result.Shape, &sphere1);
}

TEST(SphereSoaIntersectionTests, RayOutsideShapesAndPointingBackwards_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2};
    Sphere sphere2{{12, 0, 0}, 2};
    Sphere sphere3{{14, 0, 0}, 2};
    Sphere sphere4{{16, 0, 0}, 2};
    Sphere sphere5{{18, 0, 0}, 2};
    Sphere sphere6{{20, 0, 0}, 2};
    Sphere sphere7{{22, 0, 0}, 2};
    Sphere sphere8{{24, 0, 0}, 2};

    SphereSoa sphereSoa{};

    sphereSoa.AddSphere(&sphere2);
    sphereSoa.AddSphere(&sphere3);
    sphereSoa.AddSphere(&sphere4);
    sphereSoa.AddSphere(&sphere5);
    sphereSoa.AddSphere(&sphere6);
    sphereSoa.AddSphere(&sphere1); // This is the sphere that will be hit.
    sphereSoa.AddSphere(&sphere7);
    sphereSoa.AddSphere(&sphere8);

    sphereSoa.Finalize();

    Ray ray{{2, 0, 0}, {-1, 0, 0}};

    // Act
    IntersectionResult<Sphere> result = sphereSoa.Intersect(ray);

    // Assert
    EXPECT_EQ(result.Distance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.Shape, nullptr);
}

TEST(SphereSoaIntersectionTests, RayOutsideShapes_Misses_ReturnsInfinity)
{
    // Arrange
    Sphere sphere1{{10, 0, 0}, 2};
    Sphere sphere2{{12, 0, 0}, 2};
    Sphere sphere3{{14, 0, 0}, 2};
    Sphere sphere4{{16, 0, 0}, 2};
    Sphere sphere5{{18, 0, 0}, 2};
    Sphere sphere6{{20, 0, 0}, 2};
    Sphere sphere7{{22, 0, 0}, 2};
    Sphere sphere8{{24, 0, 0}, 2};

    SphereSoa sphereSoa{};

    sphereSoa.AddSphere(&sphere2);
    sphereSoa.AddSphere(&sphere3);
    sphereSoa.AddSphere(&sphere4);
    sphereSoa.AddSphere(&sphere5);
    sphereSoa.AddSphere(&sphere6);
    sphereSoa.AddSphere(&sphere1); // This is the sphere that will be hit.
    sphereSoa.AddSphere(&sphere7);
    sphereSoa.AddSphere(&sphere8);

    sphereSoa.Finalize();

    Ray ray{{2, 0, 0}, {0, 1, 0}};

    // Act
    IntersectionResult<Sphere> result = sphereSoa.Intersect(ray);

    // Assert
    EXPECT_EQ(result.Distance, std::numeric_limits<float>::infinity());
    EXPECT_EQ(result.Shape, nullptr);
}