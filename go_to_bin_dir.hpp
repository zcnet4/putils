#pragma once

#include <string>

#ifdef __unix__

#include <unistd.h>

#elif defined(_WIN32)

#include <windows.h>

#endif

namespace putils
{
    // Go to the directory containing file in currentPath
    inline void goToBinDir(std::string const& currentPath) noexcept
    {
        size_t last = currentPath.find_last_of("/");
        chdir(currentPath.substr(0, last).data());
    }
}