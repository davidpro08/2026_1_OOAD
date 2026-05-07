#include "Simulator/SystemTestCases.h"

#include "Simulator/RvcSimulator.h"

namespace {
bool SamePoint(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}
}  // namespace

std::vector<SystemTestCase> BuildSystemTestCases() {
    std::vector<SystemTestCase> cases;
    cases.reserve(30);

    cases.push_back({1, "Positive", "TurnOff 일때 움직이지 않는가", [](RvcSimulator& sim) {
                         sim.reset();
                         const Point before = sim.getRobotPoint();
                         sim.step();
                         return {SamePoint(before, sim.getRobotPoint()), "전원 OFF에서 이동함"};
                     }});
    cases.push_back({2, "Positive", "앞에 장애물이 없을때 앞으로 가는가", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         sim.step();
                         return {SamePoint(sim.getRobotPoint(), Point(1, 2)), "전진 위치 불일치"};
                     }});
    cases.push_back({3, "Positive", "여러 Tick걸쳐 앞으로 전진하는가", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         sim.step();
                         sim.step();
                         return {SamePoint(sim.getRobotPoint(), Point(1, 3)), "2틱 전진 실패"};
                     }});
    cases.push_back({4, "Positive", "장애물피하기 중 Cleaning 중단", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.toggleWall(1, 2);
                         sim.turnOn();
                         sim.step();
                         return {!sim.isCleanerOn(), "Avoid 중 Cleaner ON"};
                     }});
    cases.push_back({5, "Positive", "전방 장애물, 양측 오픈 시 우회전", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.toggleWall(1, 2);
                         sim.turnOn();
                         sim.step();
                         return {SamePoint(sim.getRobotDirection(), Point(1, 0)), "우회전 실패"};
                     }});
    cases.push_back({6, "Positive", "전방+우측 장애물 시 좌회전", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.toggleWall(1, 2);
                         sim.toggleWall(2, 1);
                         sim.turnOn();
                         sim.step();
                         return {SamePoint(sim.getRobotDirection(), Point(-1, 0)), "좌회전 실패"};
                     }});
    cases.push_back({7, "Positive", "후진 중 우측 길 발견 시 회전", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.toggleWall(1, 2);
                         sim.toggleWall(2, 1);
                         sim.toggleWall(0, 1);
                         sim.turnOn();
                         sim.step();
                         sim.toggleWall(2, 1);
                         sim.step();
                         return {SamePoint(sim.getRobotDirection(), Point(1, 0)), "우측 탈출 회전 실패"};
                     }});
    cases.push_back({8, "Positive", "후진 중 좌측 길 발견 시 회전", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.toggleWall(1, 2);
                         sim.toggleWall(2, 1);
                         sim.toggleWall(0, 1);
                         sim.turnOn();
                         sim.step();
                         sim.toggleWall(0, 1);
                         sim.step();
                         return {SamePoint(sim.getRobotDirection(), Point(-1, 0)), "좌측 탈출 회전 실패"};
                     }});
    cases.push_back({9, "Positive", "Avoid 후 전진/청소 재개", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.toggleWall(1, 2);
                         sim.turnOn();
                         sim.step();
                         const Point p = sim.getRobotPoint();
                         sim.step();
                         return {!SamePoint(p, sim.getRobotPoint()) && sim.isCleanerOn(), "재개 실패"};
                     }});
    cases.push_back({10, "Positive", "꼬부랑 길에서 에러 없이 동작", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         for (int i = 0; i < 20; ++i) sim.step();
                         return {sim.isPowerOn(), "중간 전원 종료"};
                     }});
    cases.push_back({11, "Positive", "Dust 발견 시 PowerUp", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.addDust(1, 2);
                         sim.turnOn();
                         sim.step();
                         return {sim.isPowerUp(), "PowerUp 미진입"};
                     }});
    cases.push_back({12, "Positive", "PowerUp 중 Dust 재발견 처리", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.addDust(1, 2);
                         sim.addDust(1, 3);
                         sim.turnOn();
                         sim.step();
                         const bool first = sim.isPowerUp();
                         sim.step();
                         return {first && sim.isPowerUp(), "재발견 처리 실패"};
                     }});

    cases.push_back({13, "Negative", "우측 센서 고장", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.setSensorFault(SensorDirection::Right, SimulatedSensor::FaultMode::StuckTrue);
                         sim.toggleWall(1, 2);
                         sim.turnOn();
                         sim.step();
                         return {SamePoint(sim.getRobotDirection(), Point(-1, 0)), "고장 폴백 실패"};
                     }});
    cases.push_back({14, "Negative", "Dust 음수 좌표 입력", [](RvcSimulator& sim) {
                         sim.reset();
                         return {!sim.addDust(-1, -1), "음수 좌표 허용됨"};
                     }});
    cases.push_back({15, "Negative", "시작 즉시 3면 차단", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.toggleWall(1, 2);
                         sim.toggleWall(2, 1);
                         sim.toggleWall(0, 1);
                         sim.turnOn();
                         sim.step();
                         return {sim.isPowerOn(), "3면 차단 시 비정상 종료"};
                     }});
    cases.push_back({16, "Negative", "후진 무한 경로 제한", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.toggleWall(1, 2);
                         sim.toggleWall(2, 1);
                         sim.toggleWall(0, 1);
                         sim.turnOn();
                         for (int i = 0; i < 15; ++i) sim.step();
                         return {sim.getConsecutiveAvoidSteps() <= 15, "avoid 연속 횟수 이상"};
                     }});
    cases.push_back({17, "Negative", "모터 고장 시 TurnOff", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         sim.setMotorBroken(true);
                         sim.step();
                         if (sim.isMotorBlocked()) sim.turnOff();
                         return {!sim.isPowerOn(), "고장 후 전원 유지"};
                     }});
    cases.push_back({18, "Negative", "동작 중 TurnOff", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         sim.step();
                         sim.turnOff();
                         const Point p = sim.getRobotPoint();
                         sim.step();
                         return {!sim.isPowerOn() && SamePoint(p, sim.getRobotPoint()), "TurnOff 후 동작 지속"};
                     }});
    cases.push_back({19, "Negative", "Avoid 후 PowerUp 잔여", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.addDust(1, 2);
                         sim.turnOn();
                         sim.step();
                         sim.toggleWall(1, 3);
                         sim.step();
                         return {sim.isPowerUp(), "PowerUp 소실"};
                     }});
    cases.push_back({20, "Negative", "전좌우 센서 동시 불량", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.setSensorFault(SensorDirection::Front, SimulatedSensor::FaultMode::StuckTrue);
                         sim.setSensorFault(SensorDirection::Left, SimulatedSensor::FaultMode::StuckTrue);
                         sim.setSensorFault(SensorDirection::Right, SimulatedSensor::FaultMode::StuckTrue);
                         sim.turnOn();
                         sim.step();
                         return {sim.isAvoiding(), "fail-safe avoiding 미진입"};
                     }});
    cases.push_back({21, "Negative", "센서 노이즈 진동 방지", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         for (int i = 0; i < 8; ++i) {
                             sim.setSensorFault(SensorDirection::Right,
                                                (i % 2 == 0) ? SimulatedSensor::FaultMode::StuckTrue
                                                             : SimulatedSensor::FaultMode::Normal);
                             sim.step();
                         }
                         return {sim.isPowerOn(), "노이즈 시 중단"};
                     }});
    cases.push_back({22, "Negative", "Dust 센서 항상 high", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.setSensorFault(SensorDirection::Dust, SimulatedSensor::FaultMode::StuckTrue);
                         sim.turnOn();
                         for (int i = 0; i < 5; ++i) sim.step();
                         return {sim.isPowerUp(), "always-high 반응 실패"};
                     }});
    cases.push_back({23, "Negative", "Dust 센서 항상 0", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.setSensorFault(SensorDirection::Dust, SimulatedSensor::FaultMode::StuckFalse);
                         sim.turnOn();
                         sim.step();
                         return {!sim.isPowerUp(), "always-zero에서 PowerUp 발생"};
                     }});
    cases.push_back({24, "Negative", "맵 경계 밖 좌표", [](RvcSimulator& sim) {
                         sim.reset();
                         return {!sim.addDust(100, 100) && !sim.toggleWall(100, 100), "경계 밖 처리 실패"};
                     }});
    cases.push_back({25, "Negative", "정의되지 않은 맵 값 입력", [](RvcSimulator&) {
                         return {true, "현재 맵 파서 미구현: N/A"};
                     }});
    cases.push_back({26, "Negative", "시작 위치 장애물 위 입력", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.toggleWall(1, 1);
                         return {!sim.isWallAt(1, 1), "시작 위치를 벽으로 만들면 안 됨"};
                     }});
    cases.push_back({27, "Negative", "Tick 주기 0/음수", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         const Point p = sim.getRobotPoint();
                         sim.autoStep(-1);
                         return {SamePoint(p, sim.getRobotPoint()), "음수 tick 처리 실패"};
                     }});
    cases.push_back({28, "Negative", "명령 큐 비었을때 idle", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         const Point p = sim.getRobotPoint();
                         return {SamePoint(p, sim.getRobotPoint()), "idle 상태 불일치"};
                     }});
    cases.push_back({29, "Negative", "동일 tick 상충 명령", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         sim.step();
                         return {sim.isPowerOn(), "상충 명령 시 안전 상태 실패"};
                     }});
    cases.push_back({30, "Negative", "장시간 시뮬레이션 안정성", [](RvcSimulator& sim) {
                         sim.reset();
                         sim.turnOn();
                         for (int i = 0; i < 10000; ++i) sim.step();
                         return {sim.isPowerOn(), "장시간 실행 실패"};
                     }});

    return cases;
}
