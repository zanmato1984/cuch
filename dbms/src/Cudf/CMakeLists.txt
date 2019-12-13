include(${ClickHouse_SOURCE_DIR}/cmake/dbms_glob_sources.cmake)
add_headers_and_sources(clickhouse_cudf .)

add_library(clickhouse_cudf ${clickhouse_cudf_headers} ${clickhouse_cudf_sources})

target_link_libraries(clickhouse_cudf PUBLIC clickhouse_common_io ${CUDA_LIBRARY} ${CUDA_RT_LIBRARY} ${RMM_LIBRARY} ${CUDF_LIBRARY} ${NVCATEGORY_LIBRARY} ${NVSTRINGS_LIBRARY} ${NVTEXT_LIBRARY})
target_include_directories(clickhouse_cudf PUBLIC ${DBMS_INCLUDE_DIR} ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES} ${CUDA_INCLUDE_DIR} ${CUDF_INCLUDE_DIR} ${CUDF_LIBCUDACXX_INCLUDE_DIR} ${CUDF_CUB_INCLUDE_DIR} ${CUDF_JITIFY_INCLUDE_DIR})
