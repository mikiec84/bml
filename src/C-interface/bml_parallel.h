/** \file */

#ifndef __BML_PARALLEL_H
#define __BML_PARALLEL_H

#ifdef DO_MPI
#include <mpi.h>
#endif

#include "bml_types.h"

static int myRank = 0;
static int nRanks = 1;
#ifdef DO_MPI
static MPI_Request* requestList;
static MPI_Comm ccomm;
#endif
static int* rUsed;
static int reqCount = 0;

#ifdef DO_MPI
#ifdef SINGLE
#define REAL_MPI_TYPE MPI_FLOAT
#else
#define REAL_MPI_TYPE MPI_DOUBLE
#endif

#endif

// Return total number of processors.
int bml_getNRanks(void);

// Return local rank.
int bml_getMyRank(void);

// Initialize from Fortran MPI
#ifdef DO_MPI
void bml_initParallelF(MPI_Fint fcomm);
#endif

// Return non-zero if printing occurs from this rank.
int bml_printRank(void);

// Wrapper for MPI_Init.
void bml_initParallel(int *argc, char ***argv);

// Wrapper for MPI_Finalize.
void bml_shutdownParallel(void);

// Wrapper for MPI_Barrier.
void bml_barrierParallel(void);

// Wrapper for MPI_allGatherV
void bml_allGatherVParallel(bml_matrix_t * A);

#endif