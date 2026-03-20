#include "2026_1_OOAD.h" // 상대 경로 대신 CMake include path 활용
#include <gtest/gtest.h>

TEST(SampleTest, TrueIsTrue) {
    EXPECT_TRUE(true);
}

TEST(MathTest, SampleAddWorks) {
    EXPECT_EQ(SampleAdd(1, 2), 3);
}

// main 함수는 CMake에서 GTest::gtest_main을 링크하면 자동으로 생성되므로 삭제 가능합니다.