cmake_minimum_required(VERSION 3.12.0)
project(pascal_jit_compiler VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
include(CTest)
set(CMAKE_CTEST_ARGUMENTS "--verbose")
enable_testing()

aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src sources)

add_executable(pascal_jit_compiler ${sources} main.cpp)

if(ENABLE_TEST)
add_subdirectory(tests)
endif()
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
