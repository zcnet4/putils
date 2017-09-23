#include "gtest/gtest.h"
#include "same_sign.hpp"

TEST(SameSign, Positive)
{
    EXPECT_TRUE(putils::sameSign(21, 42));
}

TEST(SameSign, Negative)
{
    EXPECT_TRUE(putils::sameSign(-21, -42));
}

TEST(SameSign, PositiveAndNegative)
{
    EXPECT_FALSE(putils::sameSign(-21, 42));
    EXPECT_FALSE(putils::sameSign(42, -21));
}

TEST(SameSign, PositiveAndZero)
{
    EXPECT_TRUE(putils::sameSign(0, 42));
    EXPECT_TRUE(putils::sameSign(42, 0));
}

TEST(SameSign, NegativeAndZero)
{
    EXPECT_TRUE(putils::sameSign(0, -42));
    EXPECT_TRUE(putils::sameSign(-42, 0));
}