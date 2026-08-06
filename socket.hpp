#include <boost/asio.hpp>
#include <openssl/rand.h>
#include <iostream>

#ifndef SOCKET
#define SOCKET

using boost::asio::ip::tcp;
class Socket
{
private:
    tcp::socket socket;
    std::string socket_id;

    void assign_id()
    {
        unsigned char id[16];
        if (RAND_bytes(id, sizeof(id)) == 1)
        {
            char *readable_id = OPENSSL_buf2hexstr(id, sizeof(id));

            socket_id = std::move(readable_id);
        }
        else
            std::cout << "could not assign id" << std::endl;
    }

public:
    Socket(boost::asio::io_context &io) : socket(io)
    {
        assign_id();
    }

    tcp::socket &give_socket()
    {
        return socket;
    }

    const std::string give_socket_id() noexcept
    {
        return socket_id;
    }

    Socket(const Socket &) = delete;
    Socket &operator=(const Socket &) = delete;

    Socket(Socket &&) = default;
    Socket &operator=(Socket &&) = default;

    ~Socket() = default;
};
#endif