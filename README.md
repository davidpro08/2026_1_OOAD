# 2026_1_OOAD 프로젝트

객체지향 분석 및 설계 수업을 위한 C++ 로봇 청소기 시뮬레이터 프로젝트입니다. CMake로 빌드하며, Google Test 기반 단위 테스트를 포함합니다.

## 프로젝트 구조

```text
2026_1_OOAD/
├─ CMakeLists.txt
├─ CMakePresets.json
├─ README.md
├─ sonar-project.properties
├─ 2026_1_OOAD/
│  ├─ CMakeLists.txt
│  ├─ main.cpp
│  ├─ Class/
│  ├─ HDWARE/
│  └─ Simulator/
├─ tests/
│  ├─ CMakeLists.txt
│  └─ test_*.cpp
└─ docs/
```

## 빌드 방법

Visual Studio에서 루트 `CMakeLists.txt`를 열거나, 명령줄에서 다음 명령을 실행합니다.

```bash
cmake --preset x64-debug
cmake --build out/build/x64-debug
```

Visual Studio 번들 CMake만 PATH에 잡힌 환경에서는 다음처럼 `cmake.exe` 전체 경로를 사용할 수 있습니다.

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build out\build\x64-debug
```

## 테스트 방법

```bash
ctest --test-dir out/build/x64-debug --output-on-failure
```

또는 테스트 실행 파일을 직접 실행할 수 있습니다.

```powershell
out\build\x64-debug\tests\unit_tests.exe
```

## 실행 방법

빌드 후 생성된 실행 파일을 실행하면 CLI 기반 로봇 청소기 시뮬레이터를 사용할 수 있습니다.

```powershell
out\build\x64-debug\2026_1_OOAD\2026_1_OOAD.exe
```

주요 명령은 `on`, `off`, `step`, `auto N`, `front`, `dust x y`, `wall x y`, `status`, `reset`, `randmap`, `help`, `quit`입니다.

## 개발 메모

- C++20을 사용합니다.
- 테스트는 Google Test `v1.14.0`을 FetchContent로 가져옵니다.
- 빌드 산출물과 Visual Studio 임시 파일은 `.gitignore` 대상입니다.
- 컨트롤러 간 이벤트는 `EventBus`를 통해 전달됩니다.
