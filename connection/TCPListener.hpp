#pragma once

#ifdef __unix__
# include <signal.h>
#endif

#include "ATCPListener.hpp"

namespace putils
{
    class TCPListener final : public ATCPListener
    {
    public:
        TCPListener(short port, std::string_view host = "127.0.0.1", bool verbose = false)
                : ATCPListener(port, host.data(), verbose)
        {}

    private:
        void doRemove(int) noexcept {}

        bool doAccept(int) noexcept { return true; }

        int doRead(int fd, char *dest, size_t length) noexcept { return (int) recv(fd, dest, length, 0); }

        int doWrite(int fd, const char *data, size_t length) noexcept { return (int) ::send(fd, data, length, 0); }
    };

    namespace test
    {
        inline int tcpListener()
        {
            // Listen on port 4242
            putils::TCPListener server(4242);
            std::cout << "Server listening on port 4242" << std::endl;

            // Observe server for new clients
            server.addObserver([&server]()
            {
                auto &client = server.getLastClient();
                // Capture client's fd as the client reference may be invalidated

                std::cout << "New client connected" << std::endl;
                client.send("Welcome!\n");

                client.newMessage += [&client]
                {
                    std::cout << "[" << client.getMsg() << "]" << std::endl;
                    client.send("Hello there!\n");
                };

                client.disconnected += []
                {
                    std::cout << "Client disconnected" << std::endl;
                };
            });

            while (true)
                server.select();
        }
    }
}