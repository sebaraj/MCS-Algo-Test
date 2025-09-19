include(GoogleTest)
file(GLOB_RECURSE TEST_FILES "*.cpp")
add_executable(tests "${TEST_FILES}")

target_link_libraries(tests
  PRIVATE
    mcis
    ${CONAN_LIBS}
    ${GTest_LIBRARIES}
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(tests)
