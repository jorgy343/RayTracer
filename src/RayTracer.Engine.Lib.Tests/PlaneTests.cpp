#include "pch.h"

import <limits>;

import Plane;

using namespace RayTracer;

class PlaneIntersectionTests : public testing::TestWithParam<std::tuple<Plane, Ray, float>>
{

};

TEST_P(PlaneIntersectionTests, Intersects_ReturnsNearestIntersection)
{
    // Arrange
    Plane plane = std::get<0>(GetParam());
    Ray ray = std::get<1>(GetParam());
    float expectedResult = std::get<2>(GetParam());

    // Act
    IntersectionResult result = plane.IntersectEntrance(ray);

    // Assert
    if (expectedResult == std::numeric_limits<float>::infinity())
    {
        EXPECT_EQ(result.HitDistance, expectedResult);
    }
    else
    {
        EXPECT_NEAR(result.HitDistance, expectedResult, 0.01f);
    }
}

INSTANTIATE_TEST_CASE_P(PlaneTests, PlaneIntersectionTests, testing::Values(
    // Intersects
    //
    // Intersects on +x axis.
    std::make_tuple(
        Plane{{1, 0, 0}, {10, 0, 0}, nullptr},
        Ray{{2, 0, 0}, {1, 0, 0}},
        8.0f),
    std::make_tuple(
        Plane{{-1, 0, 0}, {10, 0, 0}, nullptr},
        Ray{{2, 0, 0}, {1, 0, 0}},
        8.0f),

    // Intersects on -x axis.
    std::make_tuple(
        Plane{{1, 0, 0}, {-10, 0, 0}, nullptr},
        Ray{{-2, 0, 0}, {-1, 0, 0}},
        8.0f),
    std::make_tuple(
        Plane{{-1, 0, 0}, {-10, 0, 0}, nullptr},
        Ray{{-2, 0, 0}, {-1, 0, 0}},
        8.0f),

    // Intersects on +y axis.
    std::make_tuple(
        Plane{{0, 1, 0}, {0, 10, 0}, nullptr},
        Ray{{0, 2, 0}, {0, 1, 0}},
        8.0f),
    std::make_tuple(
        Plane{{0, -1, 0}, {0, 10, 0}, nullptr},
        Ray{{0, 2, 0}, {0, 1, 0}},
        8.0f),

    // Intersects on -y axis.
    std::make_tuple(
        Plane{{0, 1, 0}, {0, -10, 0}, nullptr},
        Ray{{0, -2, 0}, {0, -1, 0}},
        8.0f),
    std::make_tuple(
        Plane{{0, -1, 0}, {0, -10, 0}, nullptr},
        Ray{{0, -2, 0}, {0, -1, 0}},
        8.0f),

    // Intersects on +z axis.
    std::make_tuple(
        Plane{{0, 0, 1}, {0, 0, 10}, nullptr},
        Ray{{0, 0, 2}, {0, 0, 1}},
        8.0f),
    std::make_tuple(
        Plane{{0, 0, -1}, {0, 0, 10}, nullptr},
        Ray{{0, 0, 2}, {0, 0, 1}},
        8.0f),

    // Intersects on -z axis.
    std::make_tuple(
        Plane{{0, 0, 1}, {0, 0, -10}, nullptr},
        Ray{{0, 0, -2}, {0, 0, -1}},
        8.0f),
    std::make_tuple(
        Plane{{0, 0, -1}, {0, 0, -10}, nullptr},
        Ray{{0, 0, -2}, {0, 0, -1}},
        8.0f),

    // Misses
    //
    // Misses on +x axis.
    std::make_tuple(
        Plane{{1, 0, 0}, {10, 0, 0}, nullptr},
        Ray{{2, 0, 0}, {-1, 0, 0}},
        std::numeric_limits<float>::infinity()),
    std::make_tuple(
        Plane{{-1, 0, 0}, {10, 0, 0}, nullptr},
        Ray{{2, 0, 0}, {-1, 0, 0}},
        std::numeric_limits<float>::infinity()),

    // Misses on -x axis.
    std::make_tuple(
        Plane{{1, 0, 0}, {-10, 0, 0}, nullptr},
        Ray{{-2, 0, 0}, {1, 0, 0}},
        std::numeric_limits<float>::infinity()),
    std::make_tuple(
        Plane{{-1, 0, 0}, {-10, 0, 0}, nullptr},
        Ray{{-2, 0, 0}, {1, 0, 0}},
        std::numeric_limits<float>::infinity()),

    // Misses on +y axis.
    std::make_tuple(
        Plane{{0, 1, 0}, {0, 10, 0}, nullptr},
        Ray{{0, 2, 0}, {0, -1, 0}},
        std::numeric_limits<float>::infinity()),
    std::make_tuple(
        Plane{{0, -1, 0}, {0, 10, 0}, nullptr},
        Ray{{0, 2, 0}, {0, -1, 0}},
        std::numeric_limits<float>::infinity()),

    // Misses on -y axis.
    std::make_tuple(
        Plane{{0, 1, 0}, {0, -10, 0}, nullptr},
        Ray{{0, -2, 0}, {0, 1, 0}},
        std::numeric_limits<float>::infinity()),
    std::make_tuple(
        Plane{{0, -1, 0}, {0, -10, 0}, nullptr},
        Ray{{0, -2, 0}, {0, 1, 0}},
        std::numeric_limits<float>::infinity()),

    // Misses on +z axis.
    std::make_tuple(
        Plane{{0, 0, 1}, {0, 0, 10}, nullptr},
        Ray{{0, 0, 2}, {0, 0, -1}},
        std::numeric_limits<float>::infinity()),
    std::make_tuple(
        Plane{{0, 0, -1}, {0, 0, 10}, nullptr},
        Ray{{0, 0, 2}, {0, 0, -1}},
        std::numeric_limits<float>::infinity()),

    // Misses on -z axis.
    std::make_tuple(
        Plane{{0, 0, 1}, {0, 0, -10}, nullptr},
        Ray{{0, 0, -2}, {0, 0, 1}},
        std::numeric_limits<float>::infinity()),
    std::make_tuple(
        Plane{{0, 0, -1}, {0, 0, -10}, nullptr},
        Ray{{0, 0, -2}, {0, 0, 1}},
        std::numeric_limits<float>::infinity())
));