#ifndef __TYPED_H
#define __TYPED_H

#if defined(SINGLE_REAL)
#define REAL_T float
#define MAGMA_T float
#define FUNC_SUFFIX single_real
#define MATRIX_PRECISION single_real
#define BLAS_PREFIX S
#define MAGMA_PREFIX s
#define REAL_PART(x) (x)
#define IMAGINARY_PART(x) (0)
#define COMPLEX_CONJUGATE(x) (x)
#define ABS(x) (fabsf(x))
#define is_above_threshold(x, t) (fabsf(x) > (float) (t))
#elif defined(DOUBLE_REAL)
#define REAL_T double
#define MAGMA_T double
#define FUNC_SUFFIX double_real
#define MATRIX_PRECISION double_real
#define BLAS_PREFIX D
#define MAGMA_PREFIX d
#define REAL_PART(x) (x)
#define IMAGINARY_PART(x) (0)
#define COMPLEX_CONJUGATE(x) (x)
#define ABS(x) (fabs(x))
#define is_above_threshold(x, t) (fabs(x) > (double) (t))
#elif defined(SINGLE_COMPLEX)
#define REAL_T float _Complex
#define MAGMA_T magmaFloatComplex
#define FUNC_SUFFIX single_complex
#define MATRIX_PRECISION single_complex
#define BLAS_PREFIX C
#define MAGMA_PREFIX c
#define REAL_PART(x) (crealf(x))
#define IMAGINARY_PART(x) (cimagf(x))
#define COMPLEX_CONJUGATE(x) (conjf(x))
#define ABS(x) (cabsf(x))
#define is_above_threshold(x, t) (cabsf(x) > (float) (t))
#elif defined(DOUBLE_COMPLEX)
#define REAL_T double _Complex
#define MAGMA_T magmaDoubleComplex
#define FUNC_SUFFIX double_complex
#define MATRIX_PRECISION double_complex
#define BLAS_PREFIX Z
#define MAGMA_PREFIX z
#define REAL_PART(x) (creal(x))
#define IMAGINARY_PART(x) (cimag(x))
#define COMPLEX_CONJUGATE(x) (conj(x))
#define ABS(x) (cabs(x))
#define is_above_threshold(x, t) (cabs(x) > (double) (t))
#else
#error Unknown precision type
#endif

#define CONCAT2_(a, b) a ## _ ## b
#define CONCAT_(a, b) CONCAT2_(a, b)

#define CONCAT2(a, b) a ## b
#define CONCAT(a, b) CONCAT2(a, b)

#define TYPED_FUNC(a) CONCAT_(a, FUNC_SUFFIX)
#define C_BLAS(a) CONCAT_(C, CONCAT(BLAS_PREFIX , a))
#define MAGMACOMPLEX(a) CONCAT_(MAGMA, CONCAT_(BLAS_PREFIX, a))
#define MAGMA(a) CONCAT_(magma, CONCAT(MAGMA_PREFIX , a))
#define MAGMAGPU(a) CONCAT_(magma, CONCAT(MAGMA_PREFIX , CONCAT_(a, gpu)))
#define MAGMABLAS(a) CONCAT_(magmablas, CONCAT(MAGMA_PREFIX , a))
#endif
