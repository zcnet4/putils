#pragma once

#include <string>
#include <string_view>
#include "runTests.hpp"

namespace putils
{
    inline std::string chop(std::string_view str)
    {
        auto end = str.find_last_not_of(" \t\n\r");

        if (end == std::string::npos)
            return std::string(str);
        else
            return std::string(str.substr(0, str.find_last_not_of(" \t\n\r") + 1));
    }

    namespace test
    {
        inline bool chop()
        {
            return putils::runTests(
                    "Space", []{ return putils::chop("test ") == "test"; },
                    "Tab", []{ return putils::chop("test\t") == "test"; },
                    "Newline", []{ return putils::chop("test\n") == "test"; },
                    "Carriage return", []{ return putils::chop("test\r") == "test"; },
                    "All", []{ return putils::chop("test\r \n\t") == "test"; }
            );
        }
    }
}
