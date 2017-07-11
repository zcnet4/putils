#pragma once

#include <string>
#include <string_view>

#ifdef __unix__

#include <unistd.h>

#elif defined(_WIN32)

#include <windows.h>

#endif

namespace putils
{
    // Go to the directory containing file in currentPath
    inline void goToBinDir(const std::string &currentPath) noexcept
    {
        size_t last = currentPath.find_last_of("/\\");
        const auto dest = currentPath.substr(0, last);
        chdir(dest.data());
    }
}