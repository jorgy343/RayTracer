#include "pch.h"

#include <limits>

import RayTracer.Vector4;

using namespace RayTracer;

// ************************************
// Vector4.Dot
// ************************************
class Vector4DotProductTests : public testing::TestWithParam<std::tuple<Vector4, Vector4, float>>
{

};

TEST_P(Vector4DotProductTests, Vector4DotProductTests)
{
    // Arrange
    Vector4 left = std::get<0>(GetParam());
    Vector4 right = std::get<1>(GetParam());
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

INSTANTIATE_TEST_CASE_P(Vector4Tests, Vector4DotProductTests, testing::Values(
    std::make_tuple(
        Vector4{0, 0, 0, 0},
        Vector4{0, 0, 0, 0},
        0.0f),
    std::make_tuple(
        Vector4{5, 4, 3, 8},
        Vector4{9, 1, 5, 6},
        112.0f),
    std::make_tuple(
        Vector4{std::numeric_limits<float>::infinity(), 4, 3, 8},
        Vector4{9, 1, 5, 6},
        std::numeric_limits<float>::infinity()),
    std::make_tuple(
        Vector4{-std::numeric_limits<float>::infinity(), 4, 3, 8},
        Vector4{9, 1, 5, 6},
        std::numeric_limits<float>::quiet_NaN())
));