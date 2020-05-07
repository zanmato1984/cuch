set (CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda)
set (CUDA_LIBRARY_DIR ${CUDA_TOOLKIT_ROOT_DIR}/lib64)
set (CUDA_LIBRARY_STUBS_DIR ${CUDA_TOOLKIT_ROOT_DIR}/lib64/stubs)

find_library (CUDA_LIBRARY cuda PATHS ${CUDA_LIBRARY_STUBS_DIR})
find_library (CUDA_RT_LIBRARY cudart PATHS ${CUDA_LIBRARY_DIR})

find_path (CUDA_INCLUDE_DIR NAMES cuda.h PATHS ${CUDA_TOOLKIT_ROOT_DIR}/include)

message (STATUS "Using cuda: ${CUDA_INCLUDE_DIR} : ${CUDA_LIBRARY}")
message (STATUS "Using cudart: ${CUDA_RT_LIBRARY}")

if (NOT DEFINED ENV{CONDA_PREFIX})
    message (FATAL_ERROR "CONDA_PREFIX not defined.")
else ()
    set (CONDA_PREFIX $ENV{CONDA_PREFIX})
    message (STATUS "Using conda prefix: ${CONDA_PREFIX}")
endif ()

find_library (CUDF_LIBRARY cudf PATHS ${CONDA_PREFIX}/lib)
find_library (NVCATEGORY_LIBRARY NVCategory PATHS ${CONDA_PREFIX}/lib)
find_library (NVSTRINGS_LIBRARY NVStrings PATHS ${CONDA_PREFIX}/lib)
find_library (RMM_LIBRARY rmm PATHS ${CONDA_PREFIX}/lib)

find_path (CUDF_INCLUDE_DIR NAMES cudf/cudf.h PATHS ${CONDA_PREFIX}/include)

message (STATUS "Using cudf: ${CUDF_INCLUDE_DIR} : ${CUDF_LIBRARY}")
message (STATUS "Using NVCategory: ${NVCATEGORY_LIBRARY}")
message (STATUS "Using NVStrings: ${NVSTRINGS_LIBRARY}")
message (STATUS "Using rmm: ${RMM_LIBRARY}")
