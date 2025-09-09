include(GoogleTest)
add_executable(tests
  test.cpp
)

target_link_libraries(tests
  PRIVATE
    mcs
    ${CONAN_LIBS}
    ${GTest_LIBRARIES}
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(tests)
