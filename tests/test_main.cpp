#include <gtest/gtest.h>
#include "../2026_1_OOAD/2026_1_OOAD.h"

TEST(SampleTest, TrueIsTrue) {
  EXPECT_TRUE(true);
}

TEST(MathTest, SampleAddWorks) {
  EXPECT_EQ(SampleAdd(1, 2), 3);
  EXPECT_EQ(SampleAdd(-1, 1), 0);
  EXPECT_EQ(SampleAdd(0, 0), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
