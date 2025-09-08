include(GoogleTest)
add_executable(test
  test.cpp
)

target_link_libraries(test
  PRIVATE
    src_lib
    ${CONAN_LIBS}
    ${GTest_LIBRARIES}
    GTest::gtest
    GTest::gtest_main
)

gtest_discover_tests(test)
