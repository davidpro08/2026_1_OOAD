# 2026_1_OOAD 프로젝트

이 프로젝트는 객체지향 프로그래밍(OOP) 수업을 위한 C++ 프로젝트입니다. CMake를 사용하여 빌드되며, Google Test를 이용한 단위 테스트를 포함합니다.

## 폴더 구조

프로젝트의 폴더 구조는 다음과 같습니다:

```
2026_1_OOAD/
├── CMakeLists.txt          # 메인 CMake 설정 파일
├── CMakePresets.json       # CMake 프리셋 설정
├── README.md               # 이 파일
├── sonar-project.properties # SonarQube 설정
├── 2026_1_OOAD/            # 메인 소스 코드 폴더
│   ├── CMakeLists.txt      # 서브 CMake 설정
│   ├── main.cpp            # 메인 함수
│   ├── 2026_1_OOAD.cpp     # 구현 파일
│   └── 2026_1_OOAD.h       # 헤더 파일
├── build/                  # 빌드 출력 폴더 (CMake에 의해 생성)
│   └── _deps/              # 의존성 (Google Test 등)
├── tests/                  # 테스트 코드 폴더
│   ├── CMakeLists.txt      # 테스트 CMake 설정
│   └── test_main.cpp       # 테스트 메인 파일
```

## Clone 방법

이 프로젝트를 클론하려면 다음 명령어를 사용하세요:

```bash
git clone <repository-url>
cd 2026_1_OOAD
```

## 프로젝트 환경 설정 (Visual Studio 기준)

### 1. Visual Studio 설치 확인

- Visual Studio 2019 이상 버전을 설치하세요.
- C++ 개발 워크로드를 포함하여 설치해야 합니다.
- CMake 도구가 포함되어 있는지 확인하세요 (Visual Studio Installer에서 "C++를 사용한 데스크톱 개발" 워크로드 선택).

### 2. 프로젝트 열기

1. Visual Studio를 실행합니다.
2. "파일" > "열기" > "CMake"를 선택합니다.
3. 프로젝트 루트 폴더에서 `CMakeLists.txt` 파일을 선택합니다.
4. Visual Studio가 CMake를 자동으로 구성합니다.

### 3. CMake 구성

- Visual Studio에서 프로젝트를 열면 자동으로 CMake 구성이 시작됩니다.
- 필요한 경우, "프로젝트" > "CMake 설정"에서 구성 옵션을 조정할 수 있습니다.
- Google Test 의존성은 CMakeLists.txt에 정의되어 있어 자동으로 다운로드됩니다.

## 빌드 방법

### Visual Studio에서 빌드

1. 솔루션 탐색기에서 프로젝트를 마우스 오른쪽 버튼으로 클릭합니다.
2. "빌드"를 선택합니다.
3. 또는 F7 키를 눌러 전체 솔루션을 빌드합니다.

### 명령줄에서 빌드 (선택사항)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## 디버깅 방법 (Visual Studio 기준)

### 1. 디버그 모드로 빌드

- "디버그" 구성으로 빌드되었는지 확인하세요 (기본적으로 설정됨).

### 2. 디버깅 시작

1. 메인 함수에 브레이크포인트를 설정합니다 (F9 키).
2. F5 키를 눌러 디버깅을 시작합니다.
3. 또는 "디버그" > "디버깅 시작"을 선택합니다.

### 3. 디버깅 기능 사용

- 단계별 실행: F10 (한 줄씩 실행)
- 프로시저 단위 실행: F11 (함수 내부로 들어감)
- 변수 값 확인: 마우스를 변수 위에 올리거나, "조사식" 창 사용
- 호출 스택 확인: "디버그" > "창" > "호출 스택"

## 테스팅 방법

이 프로젝트는 Google Test를 사용하여 단위 테스트를 수행합니다.

### Visual Studio에서 테스트 실행

1. "테스트" > "테스트 탐색기"를 엽니다.
2. "모두 실행" 버튼을 클릭하여 모든 테스트를 실행합니다.
3. 또는 특정 테스트를 선택하여 실행할 수 있습니다.

### 명령줄에서 테스트 실행 (선택사항)

```bash
cd build
ctest
```

또는 직접 실행 파일을 실행:

```bash
./tests/test_main.exe
```

### 테스트 작성

- `tests/` 폴더에 새로운 테스트 파일을 추가하세요.
- `CMakeLists.txt`에 테스트를 등록하세요.
- Google Test 매크로를 사용하여 테스트를 작성하세요.

## 추가 정보

- 이 프로젝트는 C++17 표준을 사용합니다.
- CMake 3.20 이상이 필요합니다.
- 문제가 발생하면 이슈를 보고해 주세요.
