# CMakeList.txt: 2026_1_OOAD에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#

# 소스 파일들을 포함하는 라이브러리 생성 (테스트에서도 참조 가능하도록 함)


file(GLOB SRC_LIST 
    "2026_1_OOAD.cpp"
    "2026_1_OOAD.h"
    "Class/*.cpp"
    "Class/*.h"
    "HDWARE/*.cpp"
    "HDWARE/*.h"
    "Simulator/*.cpp"
    "Simulator/*.h"
)

add_library(OOAD_Lib STATIC
    ${SRC_LIST}
)

target_include_directories(OOAD_Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 이 프로젝트의 실행 파일 생성
add_executable(2026_1_OOAD main.cpp  "Class/HwCleaner.h")
target_link_libraries(2026_1_OOAD PRIVATE OOAD_Lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_target_properties(OOAD_Lib PROPERTIES CXX_STANDARD 20)
  set_target_properties(2026_1_OOAD PROPERTIES CXX_STANDARD 20)
endif()

if(MSVC)
  target_compile_options(OOAD_Lib PRIVATE /utf-8)
  target_compile_options(2026_1_OOAD PRIVATE /utf-8)
endif()

if(CPPCHECK_PATH)
  set(_ooad_cppcheck "${CPPCHECK_PATH};--enable=all;--suppress=missingIncludeSystem;--inconclusive;--check-level=exhaustive")
  set_target_properties(OOAD_Lib 2026_1_OOAD PROPERTIES CXX_CPPCHECK "${_ooad_cppcheck}")
endif()

if(CLANG_TIDY_PATH)
  set(_ooad_tidy
    "${CLANG_TIDY_PATH}"
    "-checks=clang-diagnostic-*,clang-analyzer-*,google-*,modernize-*,performance-*,readability-*"
    "-header-filter=${CMAKE_SOURCE_DIR}/(2026_1_OOAD|tests)/.*"
    "-system-headers=0"
    "--extra-arg-before=--driver-mode=cl"
    "--extra-arg=-Qunused-arguments"
  )
  set_target_properties(OOAD_Lib 2026_1_OOAD PROPERTIES CXX_CLANG_TIDY "${_ooad_tidy}")
endif()
