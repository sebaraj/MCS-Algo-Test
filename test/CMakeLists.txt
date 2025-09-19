include(GoogleTest)
file(GLOB_RECURSE TEST_FILES "*.cpp")
add_executable(tests "${TEST_FILES}")

find_package(GTest REQUIRED)
target_link_libraries(tests
  PRIVATE
    mcis
    gtest::gtest
)

gtest_discover_tests(tests)
