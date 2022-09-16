#include "pch.h"

import <limits>;

import Vector2;

using namespace Yart;

// ************************************
// Vector2.Dot
// ************************************
class Vector2DotProductTests : public testing::TestWithParam<std::tuple<Vector2, Vector2, float>>
{

};

TEST_P(Vector2DotProductTests, Vector2DotProductTests)
{
    // Arrange
    Vector2 left = std::get<0>(GetParam());
    Vector2 right = std::get<1>(GetParam());
    float expectedResult = std::get<2>(GetParam());

    // Act
    float result = left * right;

    // Assert
    if (std::isnan(expectedResult))
    {
        EXPECT_TRUE(std::isnan(result));
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

INSTANTIATE_TEST_CASE_P(Vector2Tests, Vector2DotProductTests, testing::Values(
    std::make_tuple(
        Vector2{0, 0},
        Vector2{0, 0},
        0.0f),
    std::make_tuple(
        Vector2{5, 4},
        Vector2{9, 1},
        49.0f),
    std::make_tuple(
        Vector2{std::numeric_limits<float>::infinity(), 4},
        Vector2{9, 1},
        std::numeric_limits<float>::infinity()),
    std::make_tuple(
        Vector2{-std::numeric_limits<float>::infinity(), 4},
        Vector2{9, 1},
        std::numeric_limits<float>::quiet_NaN())
));