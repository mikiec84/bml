#include "../../macros.h"
#include "../typed.h"
#include "bml_allocate.h"
#include "bml_normalize.h"
#include "bml_parallel.h"
#include "bml_types.h"
#include "bml_allocate_ellpack.h"
#include "bml_normalize_ellpack.h"
#include "bml_scale_ellpack.h"
#include "bml_add_ellpack.h"
#include "bml_types_ellpack.h"

#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif

/* Normalize ellpack matrix given Gershgorin bounds.
 *
 *  \ingroup normalize_group
 *
 *  \param A The matrix
 *  \param mineval Calculated min value
 *  \param maxeval Calculated max value
 */
void TYPED_FUNC(
    bml_normalize_ellpack) (
    bml_matrix_ellpack_t * A,
    const double mineval,
    const double maxeval)
{
    double maxminusmin = maxeval - mineval;
    double gershfact = maxeval / maxminusmin;
    REAL_T scalar = (REAL_T) - 1.0 / maxminusmin;
    double threshold = 0.0;

    bml_scale_inplace_ellpack(&scalar, A);
    bml_add_identity_ellpack(A, gershfact, threshold);
}

/** Calculate Gershgorin bounds for an ellpack matrix.
 *
 *  \ingroup normalize_group
 *
 *  \param A The matrix
 *  \param nrows Number of rows to use
 *  returns mineval Calculated min value
 *  returns maxeval Calculated max value
 */
void *TYPED_FUNC(
    bml_gershgorin_ellpack) (
    const bml_matrix_ellpack_t * A)
{
    REAL_T radius, absham, dvalue;
    double emin = 100000000000.0;
    double emax = -100000000000.0;

    double *eval = bml_allocate_memory(sizeof(double) * 2);

    int N = A->N;
    int M = A->M;
    int *A_nnz = (int *) A->nnz;
    int *A_index = (int *) A->index;
    int *A_localRowMin = A->domain->localRowMin;
    int *A_localRowMax = A->domain->localRowMax;

    int myRank = bml_getMyRank();

    REAL_T rad[N];
    REAL_T dval[N];

    REAL_T *A_value = (REAL_T *) A->value;

#pragma omp parallel for default(none) \
    shared(N, M, A_nnz, A_index, A_value) \
    shared(A_localRowMin, A_localRowMax, myRank) \
    shared(rad, dval) \
    private(absham, radius, dvalue) \
    reduction(max:emax) \
    reduction(min:emin)
    //for (int i = 0; i < N; i++)
    for (int i = A_localRowMin[myRank]; i < A_localRowMax[myRank]; i++)
    {
        radius = 0.0;
        dvalue = 0.0;

        for (int j = 0; j < A_nnz[i]; j++)
        {
            if (i == A_index[ROWMAJOR(i, j, N, M)])
                dvalue = A_value[ROWMAJOR(i, j, N, M)];
            else
            {
                absham = ABS(A_value[ROWMAJOR(i, j, N, M)]);
                radius += (double) absham;
            }

        }

        dval[i] = dvalue;
        rad[i] = radius;

/*
        emax =
            (emax >
             REAL_PART(dvalue + radius) ? emax : REAL_PART(dvalue + radius));
        emin =
            (emin <
             REAL_PART(dvalue - radius) ? emin : REAL_PART(dvalue - radius));

*/
    }
    //for (int i = 0; i < N; i++)
    for (int i = A_localRowMin[myRank]; i < A_localRowMax[myRank]; i++)
    {
        if (REAL_PART(dval[i] + rad[i]) > emax)
            emax = REAL_PART(dval[i] + rad[i]);
        if (REAL_PART(dval[i] - rad[i]) < emin)
            emin = REAL_PART(dval[i] - rad[i]);

    }

    //printf("%d: emin = %e emax = %e\n", myRank, emin, emax);

#ifdef DO_MPI
    if (bml_getNRanks() > 1 && A->distribution_mode == distributed)
    {
        bml_minRealReduce(&emin);
        bml_maxRealReduce(&emax);
    }
#endif

    eval[0] = emin;
    eval[1] = emax;

    //printf("Global %d: emin = %e emax = %e\n", myRank, emin, emax);

    return eval;
}

/** Calculate Gershgorin bounds for a partial ellpack matrix.
 *
 *  \ingroup normalize_group
 *
 *  \param A The matrix
 *  \param nrows Number of rows to use
 *  returns mineval Calculated min value
 *  returns maxeval Calculated max value
 */
void *TYPED_FUNC(
    bml_gershgorin_partial_ellpack) (
    const bml_matrix_ellpack_t * A,
    const int nrows)
{
    REAL_T radius, absham, dvalue;
    double emin = 100000000000.0;
    double emax = -100000000000.0;

    double *eval = bml_allocate_memory(sizeof(double) * 2);

    int N = A->N;
    int M = A->M;
    int *A_nnz = (int *) A->nnz;
    int *A_index = (int *) A->index;

    REAL_T rad[N];
    REAL_T dval[N];

    REAL_T *A_value = (REAL_T *) A->value;

#pragma omp parallel for default(none) \
    shared(N, M, A_nnz, A_index, A_value) \
    shared(rad, dval) \
    private(absham, radius, dvalue) \
    reduction(max:emax) \
    reduction(min:emin)
    for (int i = 0; i < nrows; i++)
    {
        radius = 0.0;
        dvalue = 0.0;

        for (int j = 0; j < A_nnz[i]; j++)
        {
            if (i == A_index[ROWMAJOR(i, j, N, M)])
                dvalue = A_value[ROWMAJOR(i, j, N, M)];
            else
            {
                absham = ABS(A_value[ROWMAJOR(i, j, N, M)]);
                radius += (double) absham;
            }

        }

        dval[i] = dvalue;
        rad[i] = radius;

    }

    for (int i = 0; i < nrows; i++)
    {
        if (REAL_PART(dval[i] + rad[i]) > emax)
            emax = REAL_PART(dval[i] + rad[i]);
        if (REAL_PART(dval[i] - rad[i]) < emin)
            emin = REAL_PART(dval[i] - rad[i]);

    }

    eval[0] = emin;
    eval[1] = emax;

    return eval;
}
