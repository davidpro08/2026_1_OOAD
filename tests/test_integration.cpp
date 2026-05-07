#include "Class/EventBus.h"
#include "Class/MotorController.h"
#include "Class/SensorController.h"
#include "Class/SensorProvider.h"
#include "Class/CleanerController.h"
#include "Class/Timer.h"
#include "Class/Tick.h"

#include <gtest/gtest.h>

//Bus���� publishAvoidObstacle�� �Ͽ�����

//Bus���� publishMoveForward�� �Ͽ�����

//Bus���� publishTurnOff�� �Ͽ�����

//Bus���� publishDectedDust�� �Ͽ�����

//Bus���� publishDectedDust �� publishDectedDust�� �ѹ� �� �Ͽ�����

//Bus���� publishMoveForward �� publishAvoidObstacle �� publishMoveForward�� �Ͽ��� ��

//Bus���� publishMoveForward ��, publishDectedDust �� publishAvoidObstacle �� publishMoveForward�� �Ͽ��� ��



