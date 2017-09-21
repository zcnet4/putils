#pragma once

#include <iostream>

namespace putils
{
    template<typename ...Tests>
    void runTests(Tests &&...tests);

    template<>
    void runTests() {}

    template<typename Str, typename Test, typename ...Tests>
    void runTests(Str &&str, Test &&test, Tests &&...tests)
    {
        std::cout << str << ": " << (test() ? "[OK]" : "[KO]") << std::endl;
        runTests(std::forward<Tests>(tests)...);
    }
    
    namespace test
    {
        inline void runTests()
        {
            putils::runTests(
                "Good test", [] { return true; },
                "Bad tests", [] { return false; }
            );            
        }
    }
}
