# katric branch: LCC (lcc_func_bin_simd with CLaMPI, the configuration of the paper: --with-simd --with-clampi) as a
# static library with the calls in include/lcc_api.h, for katric's competitor adapter. Replaces autotools for this
# use; the graph500 generator and main() are not built.
cmake_minimum_required(VERSION 3.15)
project(lcc LANGUAGES C)

find_package(MPI REQUIRED COMPONENTS C)
find_package(OpenMP REQUIRED COMPONENTS C)

if(NOT TARGET clampi::clampi)
  include(FetchContent)
  FetchContent_Declare(
    clampi
    GIT_REPOSITORY https://github.com/niklas-uhl/CLaMPI
    GIT_TAG c4aa8180f430427109a231bb3e7fab2e06775826 # katric
    SYSTEM EXCLUDE_FROM_ALL)
  FetchContent_MakeAvailable(clampi)
endif()

add_library(lcc STATIC devel/src/lcc.c)
add_library(lcc::lcc ALIAS lcc)
target_include_directories(lcc PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include PRIVATE ${CMAKE_CURRENT_LIST_DIR}/devel/src)
# config.h of configure --with-simd --with-clampi
target_compile_definitions(lcc PRIVATE LCC_NO_CONFIG_H HAVE_SIMD=1 HAVE_CLAMPI=1)
target_link_libraries(lcc PUBLIC MPI::MPI_C PRIVATE OpenMP::OpenMP_C clampi::clampi m)
set_target_properties(lcc PROPERTIES POSITION_INDEPENDENT_CODE ON C_STANDARD 99)
