# CMakeList.txt: 2026_1_OOAD에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#

# 소스 파일들을 포함하는 라이브러리 생성 (테스트에서도 참조 가능하도록 함)
add_library(OOAD_Lib STATIC 
    "2026_1_OOAD.cpp"
    "2026_1_OOAD.h"
    "Class/EventBus.cpp"
    "Class/EventBus.h"
    "Class/CleanerController.cpp"
    "Class/CleanerController.h"
    "Class/PowerController.cpp"
    "Class/PowerController.h"
    "Class/SensorController.cpp"
    "Class/SensorController.h"
    "Class/MotorController.cpp"
    "Class/MotorController.h"
    "Class/Timer.cpp"
    "Class/Timer.h"
    "Class/Tick.cpp"
    "Class/Tick.h"
    "Class/IPower.h"
    "Class/SensorProvider.h"
)

# 이 프로젝트의 실행 파일 생성
add_executable(2026_1_OOAD main.cpp) 
target_link_libraries(2026_1_OOAD PRIVATE OOAD_Lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_target_properties(OOAD_Lib PROPERTIES CXX_STANDARD 20)
  set_target_properties(2026_1_OOAD PROPERTIES CXX_STANDARD 20)
endif()

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
