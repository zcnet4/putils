#pragma once

#include <regex>

bool operator==(std::string_view str, const std::regex &reg)
{
    return std::regex_match(str.data(), reg);
}

std::cmatch operator==(std::string_view str, const std::pair<std::regex, bool> &reg)
{
    std::cmatch m;
    std::regex_match(str.data(), m, reg.first);
    return m;
}

std::pair<std::regex, bool> operator""_m(const char *str, long unsigned int)
{
    return std::make_pair(std::regex(str), true);
}

std::regex operator""_r(const char *str, long unsigned int)
{
    return std::regex(str);
}

namespace putils
{
    namespace test
    {
        inline void regex()
        {
            // Get bool with _r
            assert("toto" == R"(^t.*$)"_r);

            // Get match with _m
            auto m = ("t.oto" == R"(^t\.(.*)$)"_m);

            assert(m[1] == "oto");
        }
    }
}
