#pragma once

#include <string>

#ifdef __unix__

#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>

#elif defined(_WIN32)

#include <windows.h>

#endif

namespace putils
{
    // Go to the directory containing file in currentPath
    inline void goToBinDir(std::string currentPath)
    {
#ifdef __unix__
        struct stat sb{ };
        if (lstat(currentPath.data(), &sb) == -1)
            throw std::runtime_error("Path doesn't exist");

        // If av[0] was a symbolic link, dereference it
        if (S_ISLNK(sb.st_mode))
        {
            char buf[1024];
            auto ret = readlink(currentPath.c_str(), buf, 1023);
            if (ret == -1)
                throw std::runtime_error("Could not read link");
            buf[ret] = 0;
            currentPath = buf;
        }
#endif

        size_t last = currentPath.find_last_of("/\\");
        auto dest = currentPath.substr(0, last);
        chdir(dest.data());
    }
}
