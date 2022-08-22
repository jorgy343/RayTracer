#pragma once

#include "Vcl.h"

using namespace vcl;

static inline Vec8f SimdDot(
	Vec8f const aX,
	Vec8f const bX,
	Vec8f const aY,
	Vec8f const bY,
	Vec8f const aZ,
	Vec8f const bZ)
{
	return mul_add(aX, bX, mul_add(aY, bY, aZ * bZ));
}