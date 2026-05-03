# CMakeList.txt : 최상위 CMake 프로젝트 파일, 전역 구성을 수행하고
# 여기에 하위 프로젝트를 포함합니다.
#
cmake_minimum_required (VERSION 3.15...3.27)

# 지원되는 경우 MSVC 컴파일러에 대해 핫 다시 로드 사용하도록 설정합니다.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  # Clang-cl을 사용할 때는 EditAndContinue(/ZI) 대신 ProgramDatabase(/Zi)를 쓰도록 변경
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<CONFIG:Debug,RelWithDebInfo>,ProgramDatabase,>")
  else()
      set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  endif()
endif()

project ("2026_1_OOAD")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cppcheck 설정
find_program(CPPCHECK_PATH NAMES cppcheck)
if(CPPCHECK_PATH)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK_PATH} --enable=all --suppress=missingIncludeSystem --inconclusive --check-level=exhaustive)
    message(STATUS "Cppcheck found: ${CPPCHECK_PATH}")
else()
    message(WARNING "Cppcheck not found.")
endif()

# Clang-Tidy 설정
find_program(CLANG_TIDY_PATH NAMES clang-tidy)
if(CLANG_TIDY_PATH)
    set(CMAKE_CXX_CLANG_TIDY 
        ${CLANG_TIDY_PATH} 
        "-checks=clang-diagnostic-*,clang-analyzer-*,google-*,modernize-*,performance-*,readability-*" # 검사 항목 추가
        "-header-filter=${CMAKE_SOURCE_DIR}/.*"
        "-system-headers=0"
        "--extra-arg-before=--driver-mode=cl"
        "--extra-arg=-Qunused-arguments"
    )
    message(STATUS "Clang-Tidy found: ${CLANG_TIDY_PATH}")
else()
    message(WARNING "Clang-Tidy not found.")
endif()

# Clang 컴파일러 자체의 경고도 끕니다 (안전장치)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options("-Qunused-arguments")
endif()

# 테스트 활성화
enable_testing()

# FetchContent를 사용한 Google Test 설정
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
  SYSTEM
)
# MSVC 관련 경고 방지 (컴파일러 호환성)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 하위 프로젝트를 포함합니다.
add_subdirectory ("2026_1_OOAD")

if(TARGET gtest)
    set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES
        CXX_CPPCHECK ""
        CXX_CLANG_TIDY ""
    )
endif()

# 테스트 디렉토리 추가
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory("tests")
endif()
