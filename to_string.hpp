#pragma once

#include <sstream>
#include <iomanip>
#include "read_stream.hpp"

namespace putils
{
    template<typename Obj>
    std::string to_string(Obj &&obj)
    {
        std::stringstream s;
        s << FWD(obj);
        return s.str();
    }

    inline std::string to_string(bool v)
    {
        std::stringstream s;
        s << std::boolalpha << v;
        return s.str();
    }

    inline std::string to_string(std::istream &s)
    {
        return read_stream(s);
    }

    template<typename Obj>
    Obj parse(const std::string &str)
    {
        std::stringstream s(FWD(str));

        Obj ret;
        s >> ret;
        return ret;
    };

    template<>
    inline bool parse(const std::string &str) { return str == "true"; }
}