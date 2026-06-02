#include "Simulator/SystemTestCases.h"

#include "Simulator/RvcSimulator.h"

namespace {
using Rows = std::vector<std::string>;

struct Scenario {
    Rows rows;
    Point start;
    Point direction;
};

bool SamePoint(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

Rows OpenMap(int width, int height) {
    Rows rows(height, std::string(width, '.'));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                rows[y][x] = '#';
            }
        }
    }
    return rows;
}

void Put(Rows& rows, int x, int y, char symbol) {
    const int h = static_cast<int>(rows.size());
    const int row = h - 1 - y;
    rows[row][x] = symbol;
}

void AddCase(std::vector<SystemTestCase>& cases,
             int id,
             const char* type,
             const char* name,
             const Scenario& scenario,
             std::function<SystemTestResult(RvcSimulator&)> runCore) {
    std::function<SystemTestResult(RvcSimulator&)> wrapped =
        [scenario, runCore](RvcSimulator& sim) -> SystemTestResult {
        if (!sim.loadSystemTestScenario(scenario.rows, scenario.start, scenario.direction)) {
            return {false, "시나리오 맵 로드 실패"};
        }
        sim.showCurrentScreen();
        return runCore(sim);
    };
    cases.push_back(SystemTestCase{id, type, name, wrapped});
}
}  // namespace

std::vector<SystemTestCase> BuildSystemTestCases() {
    std::vector<SystemTestCase> cases;
    cases.reserve(30);

    Scenario s1{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Scenario s2{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Scenario s3{OpenMap(7, 8), Point(2, 2), Point(0, 1)};

    Scenario s4{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Put(s4.rows, 2, 3, '#');
    Scenario s5{OpenMap(7, 7), Point(3, 2), Point(0, 1)};
    Put(s5.rows, 3, 3, '#');
    Scenario s6{OpenMap(7, 7), Point(3, 2), Point(0, 1)};
    Put(s6.rows, 3, 3, '#');
    Put(s6.rows, 2, 2, '#');

    Scenario s7{OpenMap(7, 7), Point(3, 2), Point(0, 1)};
    Put(s7.rows, 3, 3, '#');
    Put(s7.rows, 2, 2, '#');
    Scenario s8{OpenMap(7, 7), Point(3, 2), Point(0, 1)};
    Put(s8.rows, 3, 3, '#');
    Put(s8.rows, 4, 2, '#');
    Put(s8.rows, 2, 2, '#');
    Scenario s9{OpenMap(7, 7), Point(3, 2), Point(0, 1)};
    Put(s9.rows, 3, 3, '#');
    Scenario s10{
        {
            "###########",
            "#.........#",
            "#.#######.#",
            "#.#.....#.#",
            "#.#.###.#.#",
            "#.#...#.#.#",
            "#.###.#.#.#",
            "#.....#...#",
            "###########",
        },
        Point(1, 1),
        Point(1, 0)
    };

    Scenario s11{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Put(s11.rows, 2, 3, '*');
    Scenario s12{OpenMap(7, 8), Point(2, 2), Point(0, 1)};
    Put(s12.rows, 2, 3, '*');
    Put(s12.rows, 2, 4, '*');

    Scenario s13 = s6;
    Scenario s14{OpenMap(6, 6), Point(1, 1), Point(0, 1)};
    Scenario s15 = s7;
    Put(s15.rows, 4, 2, '#');
    Scenario s16{OpenMap(7, 10), Point(3, 2), Point(0, 1)};
    Put(s16.rows, 3, 3, '#');
    Put(s16.rows, 2, 2, '#');
    Put(s16.rows, 4, 2, '#');
    Scenario s17{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Scenario s18{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Scenario s19 = s11;
    Put(s19.rows, 2, 4, '#');
    Scenario s20{OpenMap(7, 7), Point(3, 2), Point(0, 1)};
    Put(s20.rows, 3, 3, '#');
    Put(s20.rows, 2, 2, '#');
    Put(s20.rows, 4, 2, '#');
    Scenario s21{OpenMap(7, 7), Point(3, 2), Point(0, 1)};
    Put(s21.rows, 3, 3, '#');
    Put(s21.rows, 2, 2, '#');
    Put(s21.rows, 4, 2, '#');
    Scenario s22{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Put(s22.rows, 2, 3, '*');
    Scenario s23 = s22;
    Scenario s24{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Scenario s25{OpenMap(5, 5), Point(1, 1), Point(0, 1)};
    Scenario s26{OpenMap(5, 5), Point(1, 1), Point(0, 1)};
    Scenario s27{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Scenario s28{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Scenario s29{OpenMap(7, 7), Point(2, 2), Point(0, 1)};
    Scenario s30{OpenMap(8, 8), Point(1, 1), Point(0, 1)};
    Put(s30.rows, 4, 4, '#');

    AddCase(cases, 1, "Positive", "TurnOff 일때 움직이지 않는가", s1, [](RvcSimulator& sim) -> SystemTestResult {
        const Point before = sim.getRobotPoint();
        sim.step();
        return {SamePoint(before, sim.getRobotPoint()), "전원 OFF에서 이동함"};
    });
    AddCase(cases, 2, "Positive", "앞에 장애물이 없을때 앞으로 가는가", s2, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step();
        return {SamePoint(sim.getRobotPoint(), Point(2, 3)), "전진 위치 불일치"};
    });
    AddCase(cases, 3, "Positive", "여러 Tick걸쳐 앞으로 전진하는가", s3, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step(); sim.step();
        return {SamePoint(sim.getRobotPoint(), Point(2, 4)), "2틱 전진 실패"};
    });
    AddCase(cases, 4, "Positive", "장애물피하기 중 Cleaning 중단", s4, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step();
        return {!sim.isCleanerOn(), "Avoid 중 Cleaner ON"};
    });
    AddCase(cases, 5, "Positive", "전방 장애물, 좌측 오픈 시 좌회전", s5, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step();
        return {SamePoint(sim.getRobotDirection(), Point(-1, 0)), "좌측 오픈 조건에서 좌회전하지 않음"};
    });
    AddCase(cases, 6, "Positive", "전방+좌측 장애물 시 우회전 확인", s6, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step();
        return {SamePoint(sim.getRobotDirection(), Point(1, 0)), "좌측이 막혔는데 우회전하지 않음"};
    });
    AddCase(cases, 7, "Positive", "우회전 후 전방 오픈 시 전진 탈출", s7, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step(); sim.step();
        return {SamePoint(sim.getRobotPoint(), Point(4, 2)) && SamePoint(sim.getRobotDirection(), Point(1, 0)) && !sim.isAvoiding(),
                "우회전 후 열린 경로로 전진하지 않음"};
    });
    AddCase(cases, 8, "Positive", "우회전 후 전방 막힘 시 후진 진입", s8, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step(); sim.step();
        return {SamePoint(sim.getRobotPoint(), Point(3, 2)) && SamePoint(sim.getRobotDirection(), Point(0, 1)) && sim.isAvoiding(),
                "우회전 후 전방 막힘 상황에서 후진 모드로 진입하지 않음"};
    });
    AddCase(cases, 9, "Positive", "Avoid 후 전진/청소 재개", s9, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step();
        const Point p = sim.getRobotPoint(); sim.step();
        return {!SamePoint(p, sim.getRobotPoint()) && sim.isCleanerOn(), "Avoid 후 전진/청소 재개"};
    });
    AddCase(cases, 10, "Positive", "S자 꼬부랑 길에서 에러 없이 동작", s10, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn();
        const Point before = sim.getRobotPoint();
        for (int i = 0; i < 30; ++i) sim.step();
        return {sim.isPowerOn() && !SamePoint(before, sim.getRobotPoint()), "S자 꼬부랑 길에서 진행하지 못함"};
    });
    AddCase(cases, 11, "Positive", "Dust 발견 시 PowerUp", s11, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step();
        return {sim.isPowerUp(), "PowerUp 미진입"};
    });
    AddCase(cases, 12, "Positive", "PowerUp 중 Dust 재발견 처리", s12, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step(); const bool first = sim.isPowerUp(); sim.step();
        return {first && sim.isPowerUp(), "PowerUp 중 Dust 재발견 처리"};
    });

    AddCase(cases, 13, "Negative", "좌측 센서 미검출 고장 시 좌회전 우선 동작", s13, [](RvcSimulator& sim) -> SystemTestResult {
        sim.setSensorFault(SensorDirection::Left, SimulatedSensor::FaultMode::StuckFalse);
        sim.turnOn(); sim.step();
        return {SamePoint(sim.getRobotDirection(), Point(-1, 0)),
                "좌측 센서 미검출 고장 상황에서 좌회전 동작이 재현되지 않음"};
    });
    AddCase(cases, 14, "Negative", "Dust 음수 좌표 입력", s14, [](RvcSimulator& sim) -> SystemTestResult {
        return {!sim.addDust(-1, -1), "음수 좌표 거부"};
    });
    AddCase(cases, 15, "Negative", "시작 즉시 3면 차단", s15, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step();
        return {sim.isPowerOn(), "시작 즉시 3면 차단"};
    });
    AddCase(cases, 16, "Negative", "3면 차단 경로에서 후진/재확인 동작", s16, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); for (int i = 0; i < 15; ++i) sim.step();
        return {sim.isPowerOn() && sim.getConsecutiveAvoidSteps() <= 15,
                "3면 차단 경로에서 안정적으로 회피하지 못함"};
    });
    AddCase(cases, 17, "Negative", "모터 고장 시 움직이지 않음", s17, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.setMotorBroken(true); sim.step(); if (sim.isMotorBlocked()) sim.turnOff();
        return {!sim.isPowerOn(), "모터 고장 시 움직이지 않음"};
    });
    AddCase(cases, 18, "Negative", "동작 중 전원 끄기", s18, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step(); sim.turnOff(); const Point p = sim.getRobotPoint(); sim.step();
        return {!sim.isPowerOn() && SamePoint(p, sim.getRobotPoint()), "동작 중 전원 끄기"};
    });
    AddCase(cases, 19, "Negative", "PowerUp 직후 장애물 회피 방향 전환", s19, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn(); sim.step(); sim.step();
        return {sim.isPowerOn() && SamePoint(sim.getRobotDirection(), Point(-1, 0)),
                "PowerUp 직후 장애물 회피 방향 전환이 확인되지 않음"};
    });
    AddCase(cases, 20, "Negative", "후진 중 좌측 센서 미검출 고장", s20, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn();
        sim.step();
        sim.step();
        sim.setSensorFault(SensorDirection::Left, SimulatedSensor::FaultMode::StuckFalse);
        sim.step();
        return {sim.isPowerOn() && !SamePoint(sim.getRobotPoint(), Point(3, 2)),
                "후진 중 좌측 센서 미검출 고장 상황에서 이동이 재현되지 않음"};
    });
    AddCase(cases, 21, "Negative", "후진 중 좌측 센서 노이즈", s21, [](RvcSimulator& sim) -> SystemTestResult {
        sim.turnOn();
        sim.step();
        sim.step();
        const Point before = sim.getRobotPoint();
        for (int i = 0; i < 6; ++i) {
            sim.setSensorFault(SensorDirection::Left,
                (i % 2 == 0) ? SimulatedSensor::FaultMode::StuckTrue : SimulatedSensor::FaultMode::Normal);
            sim.step();
        }
        return {sim.isPowerOn() && !SamePoint(before, sim.getRobotPoint()), "노이즈 상황에서 진행/복구 동작이 보이지 않음"};
    });
    AddCase(cases, 22, "Negative", "Dust 센서 항상 high", s22, [](RvcSimulator& sim) -> SystemTestResult {
        sim.setSensorFault(SensorDirection::Dust, SimulatedSensor::FaultMode::StuckTrue);
        sim.turnOn(); for (int i = 0; i < 5; ++i) sim.step();
        return {sim.isPowerUp(), "Dust가 있는 맵에서 always-high 반응 실패"};
    });
    AddCase(cases, 23, "Negative", "Dust 센서 항상 0", s23, [](RvcSimulator& sim) -> SystemTestResult {
        sim.setSensorFault(SensorDirection::Dust, SimulatedSensor::FaultMode::StuckFalse);
        sim.turnOn(); sim.step();
        return {!sim.isPowerUp(), "Dust가 있어도 always-zero 고장 반응이 안 보임"};
    });
    AddCase(cases, 24, "Negative", "음수 좌표 입력 거부", s24, [](RvcSimulator& sim) -> SystemTestResult {
        const bool dustRejected = !sim.addDust(-1, -1);
        const bool wallRejected = !sim.toggleWall(-1, -1);
        return {dustRejected && wallRejected, "음수 좌표가 허용됨"};
    });
    AddCase(cases, 25, "Negative", "맵 경계 밖 좌표 입력 거부", s25, [](RvcSimulator& sim) -> SystemTestResult {
        const bool dustRejected = !sim.addDust(100, 100);
        const bool wallRejected = !sim.toggleWall(100, 100);
        return {dustRejected && wallRejected, "맵 경계 밖 좌표가 허용됨"};
    });
    AddCase(cases, 26, "Negative", "시작 위치가 벽이면 로드 실패", s26, [](RvcSimulator& sim) -> SystemTestResult {
        const std::vector<std::string> rows = {
            "#####",
            "#...#",
            "#.#.#",
            "#...#",
            "#####"
        };
        const bool loaded = sim.loadSystemTestScenario(rows, Point(2, 2), Point(0, 1));
        return {!loaded, "벽 위 시작 위치가 허용됨"};
    });
    AddCase(cases, 27, "Negative", "시작 위치가 맵 밖이면 로드 실패", s27, [](RvcSimulator& sim) -> SystemTestResult {
        const std::vector<std::string> rows = {
            "#####",
            "#...#",
            "#...#",
            "#...#",
            "#####"
        };
        const bool loaded = sim.loadSystemTestScenario(rows, Point(5, 5), Point(0, 1));
        return {!loaded, "맵 밖 시작 위치가 허용됨"};
    });
    AddCase(cases, 28, "Negative", "행 길이 불일치 맵 입력 거부", s28, [](RvcSimulator& sim) -> SystemTestResult {
        const std::vector<std::string> rows = {
            "#####",
            "#...#",
            "#..#",
            "#####"
        };
        const bool loaded = sim.loadSystemTestScenario(rows, Point(1, 1), Point(0, 1));
        return {!loaded, "비정형 맵(행 길이 불일치)이 허용됨"};
    });
    AddCase(cases, 29, "Negative", "벽/먼지 동일 좌표 충돌 처리", s29, [](RvcSimulator& sim) -> SystemTestResult {
        const Point target(3, 3);
        const bool dustAdded = sim.addDust(target.x, target.y);
        const bool wallToggled = sim.toggleWall(target.x, target.y);
        const bool dustRejectedOnWall = !sim.addDust(target.x, target.y);
        return {dustAdded && wallToggled && sim.isWallAt(target.x, target.y) && dustRejectedOnWall,
                "벽과 먼지 충돌 처리(벽 우선/먼지 거부) 실패"};
    });
    AddCase(cases, 30, "Negative", "먼지 위 시작 위치 시나리오 처리", s30, [](RvcSimulator& sim) -> SystemTestResult {
        const std::vector<std::string> rows = {
            "#####",
            "#...#",
            "#.*.#",
            "#...#",
            "#####"
        };
        const bool loaded = sim.loadSystemTestScenario(rows, Point(2, 2), Point(0, 1));
        if (!loaded) {
            return {false, "먼지 위 시작 시나리오 로드 실패"};
        }

        sim.turnOn();
        sim.step();
        return {sim.isPowerOn(), "먼지 위 시작 후 기본 동작 유지 실패"};
    });

    return cases;
}
