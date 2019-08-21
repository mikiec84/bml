#!/bin/bash

# Make sure all the paths are correct

rm -r build
rm -r install

MY_PATH=$(pwd)
export CC=${CC:=gcc}
export FC=${FC:=gfortran}
export CXX=${CXX:=g++}
export BLAS_VENDOR=${BLAS_VENDOR:=Auto}
export BML_OPENMP=${BML_OPENMP:=yes}
export INSTALL_DIR=${INSTALL_DIR:="${MY_PATH}/install"}
export BML_TESTING=${BML_TESTING:=yes}
export BML_COMPLEX=${BML_COMPLEX:=yes}
export CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:=Release}
export MAGMA_ROOT=${MAGMA_ROOT:="${MEMBERWORK}/csc304/magma-2.5.0"}
export BML_MAGMA=${BML_MAGMA:=yes}
export EXTRA_CFLAGS=${EXTRA_CFLAGS:="-fopenmp -lpthread -I${OLCF_OPENBLAS_ROOT}/include "}
export CMAKE_Fortran_FLAGS=${CMAKE_Fortran_FLAGS:="-fopenmp -lpthread -L${OLCF_OPENBLAS_ROOT}/lib64 -lopenblas -L${OLCF_CUDA_ROOT}/lib64/ -lcublas -lcudart"}
export EXTRA_LINK_FLAGS=${EXTRA_LINK_FLAGS:="-fopenmp -lpthread -L${OLCF_OPENBLAS_ROOT}/lib64 -lopenblas -L${OLCF_CUDA_ROOT}/lib64/ -lcublas -lcudart"}

./build.sh configure


