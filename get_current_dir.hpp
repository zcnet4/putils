#pragma once

#ifdef __unix__

# include <unistd.h>
# include <string>

#endif

namespace putils
{
    // Get current directory
    template<size_t buffSize = 1024>
    std::string getCurrentDir()
    {
        char buff[buffSize];

        if (getcwd(buff, buffSize) != nullptr)
            return buff;
        return "";
    }
}