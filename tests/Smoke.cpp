#include <gtest/gtest.h>

TEST(SmokeTest, IsEqual)
{
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