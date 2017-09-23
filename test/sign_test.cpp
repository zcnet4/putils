#include "gtest/gtest.h"
#include "sign.hpp"

TEST(Sign, Positive)
{
    EXPECT_EQ(putils::sign(42), 1);
}

TEST(Sign, Negative)
{
    EXPECT_EQ(putils::sign(-42), -1);
}

TEST(Sign, Zero)
{
    EXPECT_EQ(putils::sign(0), 0);
}
