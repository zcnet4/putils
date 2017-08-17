#pragma once

#include <iostream>
#include <string_view>

#define pmeta_nameof(name) #name
#define pmeta_nameof_private(name) std::string_view(#name).substr(1)

namespace pmeta
{
    namespace test
    {
        inline void nameof()
        {
            int normalTest = 42;
            std::cout << pmeta_nameof(normalTest) << std::endl;

            int _privateTest = 84;
            std::cout << pmeta_nameof_private(_privateTest) << std::endl;
        }
    }
}