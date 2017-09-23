#include <vector>
#include <list>
#include "gtest/gtest.h"
#include "remove_if.hpp"

TEST(RemoveIf, Vector)
{
    std::vector<int> v { 1, 2, 3 };
    putils::remove_if(v, [](auto i){ return i != 2; });
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 2);
}

TEST(RemoveIf, BadValue)
{
    std::vector<int> v { 1 };
    putils::remove_if(v, [](auto i) { return i == 2; });
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 1);
}

TEST(RemoveIf, List)
{
    std::list<int> v { 1, 2, 3 };
    putils::remove_if(v, [](auto i){ return i != 2; });
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(*(v.begin()), 2);
}
