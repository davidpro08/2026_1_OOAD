include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests test_main.cpp)
target_link_libraries(unit_tests PRIVATE OOAD_Lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
