include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SRCS "*.cpp")
add_executable(unit_tests ${TEST_SRCS})

if(MSVC)
    target_link_options(unit_tests PRIVATE /DEBUG)
    target_compile_options(unit_tests PRIVATE /utf-8)
endif()

target_link_libraries(unit_tests PRIVATE OOAD_Lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
