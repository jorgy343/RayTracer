#pragma once

#define DOT256(aX, bX, aY, bY, aZ, bZ) _mm256_fmadd_ps(aX, bX, _mm256_fmadd_ps(aY, bY, _mm256_mul_ps(aZ, bZ)))

#define NEGATE256(a) _mm256_xor_ps(a, _mm256_set1_ps(-0.0f))