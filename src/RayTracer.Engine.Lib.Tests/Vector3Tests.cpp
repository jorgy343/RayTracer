#include "pch.h"

#include <limits>

import RayTracer.Vector3;

using namespace RayTracer;

// ************************************
// Vector3.Dot
// ************************************
class Vector3DotProductTests : public testing::TestWithParam<std::tuple<Vector3, Vector3, float>>
{

};

TEST_P(Vector3DotProductTests, Vector3DotProductTests)
{
    // Arrange
    Vector3 left = std::get<0>(GetParam());
    Vector3 right = std::get<1>(GetParam());
    float expectedResult = std::get<2>(GetParam());

    // Act
    float result = left * right;

    // Assert
    if (std::isnan(expectedResult))
    {
        EXPECT_TRUE(result, std::isnan(result));
    }
    else if (std::isinf(expectedResult))
    {
        EXPECT_EQ(result, expectedResult);
    }
    else
    {
        EXPECT_NEAR(result, expectedResult, 0.01f);
    }
}

INSTANTIATE_TEST_CASE_P(Vector3Tests, Vector3DotProductTests, testing::Values(
    std::make_tuple(
        Vector3{0, 0, 0},
        Vector3{0, 0, 0},
        0.0f),
    std::make_tuple(
        Vector3{5, 4, 3},
        Vector3{9, 1, 5},
        64.0f),
    std::make_tuple(
        Vector3{std::numeric_limits<float>::infinity(), 4, 3},
        Vector3{9, 1, 5},
        std::numeric_limits<float>::infinity()),
    std::make_tuple(
        Vector3{-std::numeric_limits<float>::infinity(), 4, 3},
        Vector3{9, 1, 5},
        std::numeric_limits<float>::quiet_NaN())
));

// ************************************
// Vector3.CrossProduct
// ************************************
class Vector3CrossProductTests : public testing::TestWithParam<std::tuple<Vector3, Vector3, Vector3>>
{

};

TEST_P(Vector3CrossProductTests, Vector3CrossProductTests)
{
    // Arrange
    Vector3 left = std::get<0>(GetParam());
    Vector3 right = std::get<1>(GetParam());
    Vector3 expectedResult = std::get<2>(GetParam());

    // Act
    Vector3 result = left % right;

    // Assert
    EXPECT_TRUE(expectedResult.Compare(result, 0.01f));
}

INSTANTIATE_TEST_CASE_P(Vector3Tests, Vector3CrossProductTests, testing::Values(
    std::make_tuple(
        Vector3{0, 0, 0},
        Vector3{0, 0, 0},
        Vector3{0, 0, 0}),
    std::make_tuple(
        Vector3{5, 4, 3},
        Vector3{9, 1, 5},
        Vector3{17, 2, -31}),
    std::make_tuple(
        Vector3{std::numeric_limits<float>::infinity(), 4, 3},
        Vector3{9, 1, 5},
        Vector3{17, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()}),
    std::make_tuple(
        Vector3{-std::numeric_limits<float>::infinity(), 4, 3},
        Vector3{9, 1, 5},
        Vector3{17, -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()})
));