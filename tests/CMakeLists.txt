# Google Test와 연동된 테스트 실행 파일 생성
add_executable(unit_tests 
    test_main.cpp
    test_EventBus.cpp
    test_CleanerController.cpp
    test_PowerController.cpp
    test_SensorController.cpp
    test_Timer.cpp
    test_MotorController.cpp
    test_Tick.cpp
)

# GTest 및 프로젝트 소스 연결
target_link_libraries(unit_tests GTest::gtest GTest::gtest_main OOAD_Lib)

# 헤더 경로 추가
target_include_directories(unit_tests PRIVATE 
    "../2026_1_OOAD"
    "../2026_1_OOAD/Class"
)

# C++20 표준 적용
set_target_properties(unit_tests PROPERTIES CXX_STANDARD 20)

# 테스트 실행 시 추가 옵션
include(GoogleTest)
gtest_discover_tests(unit_tests)
