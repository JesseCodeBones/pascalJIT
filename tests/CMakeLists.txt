find_package(GTest REQUIRED)
file(GLOB 
test_sources 
CONFIGURE_DEPENDS 
./*.cpp
../src/*.cpp
../src/*.hpp)

add_executable(
  pascal_jit_e2e_test
  ${test_sources}
)

target_link_libraries(
  pascal_jit_e2e_test
  GTest::gtest_main
)

gtest_discover_tests(pascal_jit_e2e_test)