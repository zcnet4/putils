#pragma once

#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "concat.hpp"

namespace putils
{
    class UnixNamedPipe : public std::fstream
    {
    public:
        UnixNamedPipe(std::string_view fileName)
        : _fileName(fileName.data())
        {
            this->open(fileName.data());
        }
        UnixNamedPipe() = default;

    public:
        ~UnixNamedPipe()
        {
            this->close();
        }

    public:
        void open(std::string_view fileName, std::ifstream::openmode mode = std::ifstream::in | std::ifstream::out)
        {
            this->close();

            mkfifo(fileName.data(), 0666); // Ignore error
            std::fstream::open(fileName.data(), mode);
            _fileName = fileName;
        }

        void close(bool erase = true)
        {
            if (!is_open())
                return;

            std::fstream::close();
            if (erase)
                system(putils::concat("rm -f ", _fileName).c_str());

            _fileName = "";
        }

    private:
        std::string _fileName;
    };
}