#pragma once

#include <iostream>
#include "ATCPConnection.hpp"

#ifdef __unix__
# include <signal.h>
#endif

namespace putils
{
    class TCPConnection final : public ATCPConnection
    {
    public:
        TCPConnection(std::string_view host, short port, bool verbose = false)
                : ATCPConnection(host, port, verbose)
        {
#ifdef __unix__
            signal(SIGPIPE, SIG_IGN);
#endif
        }

    private:
        int doRead(int fd, char *dest, int length) noexcept { return (int) recv(fd, dest, (size_t) length, 0); }

        int doWrite(int fd, const char *data, int length) noexcept { return (int) ::send(fd, data, (size_t) length, 0); }
    };

    namespace test
    {
        inline int tcpConnection()
        {
            putils::TCPConnection   sock("127.0.0.1", 4242);

            // Receive string
            std::cout << sock.receive() << std::endl;

            // Send string
            sock.send("Hi!\n");

            return 0;
        }
    }
}
