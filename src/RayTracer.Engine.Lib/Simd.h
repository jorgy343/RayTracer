#pragma once

#include <immintrin.h>

#define DOT_256(aX, bX, aY, bY, aZ, bZ) _mm256_fmadd_ps(aX, bX, _mm256_fmadd_ps(aY, bY, _mm256_mul_ps(aZ, bZ)))

#define NEGATE_256(ymmA) _mm256_xor_ps(ymmA, _mm256_set1_ps(-0.0f))

#define HORIZONTAL_MIN_256(ymmA, result) \
															/*      [upper   |   lower]																			*/ \
															/*      [7 6 5 4 | 3 2 1 0]																			*/ \
	__m256 v1 = ymmA;										/* v1 = [H G F E | D C B A]																			*/ \
	__m256 v2 = _mm256_permute_ps(v1, 0b10'11'00'01);		/* v2 = [G H E F | C D A B]																			*/ \
	__m256 v3 = _mm256_min_ps(v1, v2);						/* v3 = [W=min(G,H) W=min(G,H) Z=min(E,F) Z=min(E,F) | Y=min(C,D) Y=min(C,D) X=min(A,B) X=min(A,B)]	*/ \
															/* v3 = [W W Z Z | Y Y X X]																			*/ \
	__m256 v4 = _mm256_permute_ps(v3, 0b00'00'10'10);		/* v4 = [Z Z W W | X X Y Y]																			*/ \
	__m256 v5 = _mm256_min_ps(v3, v4);						/* v5 = [J=min(Z,W) J=min(Z,W) J=min(Z,W) J=min(Z,W) | I=min(X,Y) I=min(X,Y) I=min(X,Y) I=min(X,Y)]	*/ \
															/* v5 = [J J J J | I I I I]																			*/ \
	__m128 v6 = _mm256_extractf128_ps(v5, 1);				/* v6 = [- - - - | J J J J]																			*/ \
	__m128 v7 = _mm_min_ps(_mm256_castps256_ps128(v5), v6);	/* v7 = [- - - - | M=min(I,J) M=min(I,J) M=min(I,J) M=min(I,J)]										*/ \
															/* v7 = [- - - - | M M M M]																			*/ \
															/* M = min(I,J)																						*/ \
															/* M = min(min(X,Y),min(Z,W))																		*/ \
															/* M = min(min(min(A,B),min(C,D)),min(min(E,F),min(G,H)))											*/ \
	_mm_store_ss(&result, v7);

#define HORIZONTAL_MAX_256(ymmA, result) \
															/*      [upper   |   lower]																			*/ \
															/*      [7 6 5 4 | 3 2 1 0]																			*/ \
	__m256 v1 = ymmA;										/* v1 = [H G F E | D C B A]																			*/ \
	__m256 v2 = _mm256_permute_ps(v1, 0b10'11'00'01);		/* v2 = [G H E F | C D A B]																			*/ \
	__m256 v3 = _mm256_max_ps(v1, v2);						/* v3 = [W=max(G,H) W=max(G,H) Z=max(E,F) Z=max(E,F) | Y=max(C,D) Y=max(C,D) X=max(A,B) X=max(A,B)]	*/ \
															/* v3 = [W W Z Z | Y Y X X]																			*/ \
	__m256 v4 = _mm256_permute_ps(v3, 0b00'00'10'10);		/* v4 = [Z Z W W | X X Y Y]																			*/ \
	__m256 v5 = _mm256_max_ps(v3, v4);						/* v5 = [J=max(Z,W) J=max(Z,W) J=max(Z,W) J=max(Z,W) | I=max(X,Y) I=max(X,Y) I=max(X,Y) I=max(X,Y)]	*/ \
															/* v5 = [J J J J | I I I I]																			*/ \
	__m128 v6 = _mm256_extractf128_ps(v5, 1);				/* v6 = [- - - - | J J J J]																			*/ \
	__m128 v7 = _mm_max_ps(_mm256_castps256_ps128(v5), v6);	/* v7 = [- - - - | M=max(I,J) M=max(I,J) M=max(I,J) M=max(I,J)]										*/ \
															/* v7 = [- - - - | M M M M]																			*/ \
															/* M = max(I,J)																						*/ \
															/* M = max(max(X,Y),max(Z,W))																		*/ \
															/* M = max(max(max(A,B),max(C,D)),max(max(E,F),max(G,H)))											*/ \
	_mm_store_ss(&result, v7);