#include <gtest/gtest.h>
#include <iostream>
TEST(SmokeTest, IsEqual)
{
  std::cout << __cplusplus << std::endl;
  EXPECT_EQ(1, 1);
}

TEST(SmokeTest, IsTrue)
{
  EXPECT_TRUE(true);
}

TEST(SmokeTest, IsFalse)
{
  EXPECT_FALSE(false);
}