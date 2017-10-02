#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <comdef.h>
#include <algorithm>
#include <stdio.h>
#include <sstream>

#include "Library.hpp"

#include "get_last_error_as_string.hpp"
#include "concat.hpp"
#include "get_current_dir.hpp"

namespace putils
{
class WindowsLibrary final : public putils::Library
{
    // Constructor
public:
    WindowsLibrary(std::string_view name)
    :
    Library(name.data())// ,
    // _handle(LoadLibrary(name.c_str()))
{
    std::string copy(name);

    std::replace(copy.begin(), copy.end(), '/', '\\');
    _handle = LoadLibrary(copy.c_str());
    if (_handle == nullptr)
    {
        std::stringstream s;
        s << "Failed to load library '" << name << "': " << putils::GetLastErrorAsString();
        std::cerr << "[" << copy << "]" << std::endl;
        std::cerr << putils::getCurrentDir<>() << std::endl;
        throw std::runtime_error(s.str());
    }
}


    // Destructor
public:
    ~WindowsLibrary() noexcept
    {
        if (_handle != nullptr)
            FreeLibrary(_handle);
    }

    // Load a symbol
public:
    void *loadSymbol(std::string_view name) noexcept override { return (void*)GetProcAddress(_handle, name.data()); }

    // Attributes
private:
    HMODULE _handle;

private:
    WindowsLibrary(const WindowsLibrary &) = delete;
    WindowsLibrary &operator=(const WindowsLibrary &) = delete;
};
}

#endif
