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

# Cppcheck / Clang-Tidy: 전역(CMAKE_CXX_*)에 두면 FetchContent(googletest)까지 검사되어 빌드가 실패합니다.
# 경로만 찾아 두고, 실제 적용은 2026_1_OOAD/CMakeLists.txt에서 OOAD_Lib·실행 파일 타겟에만 합니다.
find_program(CPPCHECK_PATH NAMES cppcheck)
if(CPPCHECK_PATH)
    message(STATUS "Cppcheck found: ${CPPCHECK_PATH}")
else()
    message(WARNING "Cppcheck not found.")
endif()

find_program(CLANG_TIDY_PATH NAMES clang-tidy)
if(CLANG_TIDY_PATH)
    message(STATUS "Clang-Tidy found: ${CLANG_TIDY_PATH}")
else()
    message(WARNING "Clang-Tidy not found.")
endif()

# Clang 컴파일러 자체의 경고도 끕니다 (안전장치)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options("-Qunused-arguments")
endif()

# 하위 프로젝트를 포함합니다.
add_subdirectory ("2026_1_OOAD")

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
