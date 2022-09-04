#include "pch.h"

#include <limits>

import RayTracer.Matrix4x4;

using namespace RayTracer;

// ************************************
// Matrix4x4.Determinant
// ************************************
class Matrix4x4DeterminantTests : public testing::TestWithParam<std::tuple<Matrix4x4, float>>
{

};

TEST_P(Matrix4x4DeterminantTests, Matrix4x4DeterminantTests)
{
    // Arrange
    Matrix4x4 value = std::get<0>(GetParam());
    float expectedResult = std::get<1>(GetParam());

    // Act
    float result = value.Determinant();

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

INSTANTIATE_TEST_CASE_P(Matrix4x4Tests, Matrix4x4DeterminantTests, testing::Values(
    std::make_tuple(
        Matrix4x4
        {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        0.0f),
    std::make_tuple(
        Matrix4x4
        {
            8, 3, 8, 5,
            2, 6, 7, 9,
            9, 5, 1, 3,
            3, 5, 2, 4
        },
        452.0f),
    std::make_tuple(
        Matrix4x4
        {
            8, 3, 8, std::numeric_limits<float>::infinity(),
            2, 6, 7, 9,
            9, 5, 1, 3,
            3, 5, 2, 4
        },
        std::numeric_limits<float>::quiet_NaN()),
    std::make_tuple(
        Matrix4x4
        {
            8, 3, 8, -std::numeric_limits<float>::infinity(),
            2, 6, 7, 9,
            9, 5, 1, 3,
            3, 5, 2, 4
        },
        std::numeric_limits<float>::quiet_NaN())
));