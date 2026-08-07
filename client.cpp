#include <iostream>
#include <boost/asio.hpp>
#include <boost/url.hpp>
#include <openssl/rand.h>
#include <array>
#include "socket.hpp"

using boost::asio::ip::tcp;
class Client
{
private:
    boost::asio::io_context &io;
    tcp::resolver resolver{io};
    boost::urls::url url{"localhost"};
    std::string port{"3500"};

    Socket socket{io};

    boost::asio::awaitable<bool>
    send_message(const std::string &message)
    {
        try
        {
            // if for some reason connection drops reestablish if met with failure tell client
            if (!socket.give_socket().is_open())
            {
                const bool connected_again = co_await connect_to_server();
                if (!connected_again)
                    co_return true;
            }

            co_await boost::asio::async_write(socket.give_socket(), boost::asio::buffer(message), boost::asio::use_awaitable);
            co_return true;
        }
        catch (const boost::system::error_code &ec)
        {
            std::cout << ec.message() << std::endl;
            co_return false;
        }
    }

    boost::asio::awaitable<void>
    read_message_and_display()
    {
        try
        {
            boost::asio::streambuf read_buffer;

            co_await boost::asio::async_read_until(socket.give_socket(), read_buffer, "\nend", boost::asio::use_awaitable);

            std::cout << read_buffer.data().data() << std::endl;

            co_await read_message_and_display();
        }
        catch (const boost::system::error_code &ec)
        {
            std::cout << ec.message() << std::endl;
        }
    }

    boost::asio::awaitable<bool>
    connect_to_server()
    {
        try
        {
            auto endpoints = co_await resolver.async_resolve(url.host(), port, boost::asio::use_awaitable);
            boost::asio::async_connect(socket.give_socket(), endpoints, boost::asio::use_awaitable);
            co_return true;
        }
        catch (const boost::system::error_code &ec)
        {
            std::cout << ec.message() << std::endl;
            co_return true;
        }
    }
    Client(boost::asio::io_context &io) : io(io) {}
    ~Client() = default;

    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

    Client(Client &&) = delete;
    Client &operator=(Client &&) = delete;
};