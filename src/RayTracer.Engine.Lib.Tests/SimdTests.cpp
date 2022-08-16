#include "pch.h"

#include "Simd.h"

TEST(Simd_HoriziontalMin256, CorrectMinimumReturned1)
{
	// Arrange
	alignas(32) float values[8]
	{
		1, 2, 3, 4, 5, 6, 7, 8,
	};

	__m256 valuesXmm = _mm256_load_ps(values);

	// Act
	float result;
	HORIZONTAL_MIN_256(valuesXmm, result);

	// Assert
	EXPECT_EQ(result, 1);
}

TEST(Simd_HoriziontalMin256, CorrectMinimumReturned2)
{
	// Arrange
	alignas(32) float values[8]
	{
		2, 1, 3, 4, 5, 6, 7, 8,
	};

	__m256 valuesXmm = _mm256_load_ps(values);

	// Act
	float result;
	HORIZONTAL_MIN_256(valuesXmm, result);

	// Assert
	EXPECT_EQ(result, 1);
}

TEST(Simd_HoriziontalMin256, CorrectMinimumReturned3)
{
	// Arrange
	alignas(32) float values[8]
	{
		2, 3, 1, 4, 5, 6, 7, 8,
	};

	__m256 valuesXmm = _mm256_load_ps(values);

	// Act
	float result;
	HORIZONTAL_MIN_256(valuesXmm, result);

	// Assert
	EXPECT_EQ(result, 1);
}

TEST(Simd_HoriziontalMin256, CorrectMinimumReturned4)
{
	// Arrange
	alignas(32) float values[8]
	{
		2, 3, 4, 1, 5, 6, 7, 8,
	};

	__m256 valuesXmm = _mm256_load_ps(values);

	// Act
	float result;
	HORIZONTAL_MIN_256(valuesXmm, result);

	// Assert
	EXPECT_EQ(result, 1);
}

TEST(Simd_HoriziontalMin256, CorrectMinimumReturned5)
{
	// Arrange
	alignas(32) float values[8]
	{
		2, 3, 4, 5, 1, 6, 7, 8,
	};

	__m256 valuesXmm = _mm256_load_ps(values);

	// Act
	float result;
	HORIZONTAL_MIN_256(valuesXmm, result);

	// Assert
	EXPECT_EQ(result, 1);
}

TEST(Simd_HoriziontalMin256, CorrectMinimumReturned6)
{
	// Arrange
	alignas(32) float values[8]
	{
		2, 3, 4, 5, 6, 1, 7, 8,
	};

	__m256 valuesXmm = _mm256_load_ps(values);

	// Act
	float result;
	HORIZONTAL_MIN_256(valuesXmm, result);

	// Assert
	EXPECT_EQ(result, 1);
}

TEST(Simd_HoriziontalMin256, CorrectMinimumReturned7)
{
	// Arrange
	alignas(32) float values[8]
	{
		2, 3, 4, 5, 6, 7, 1, 8,
	};

	__m256 valuesXmm = _mm256_load_ps(values);

	// Act
	float result;
	HORIZONTAL_MIN_256(valuesXmm, result);

	// Assert
	EXPECT_EQ(result, 1);
}

TEST(Simd_HoriziontalMin256, CorrectMinimumReturned8)
{
	// Arrange
	alignas(32) float values[8]
	{
		2, 3, 4, 5, 6, 7, 8, 1,
	};

	__m256 valuesXmm = _mm256_load_ps(values);

	// Act
	float result;
	HORIZONTAL_MIN_256(valuesXmm, result);

	// Assert
	EXPECT_EQ(result, 1);
}