#include <iostream>
#include <boost/asio.hpp>
#include <openssl/rand.h>
#include <array>
#include "socket.hpp"

using boost::asio::ip::tcp;
class Client
{
private:
    boost::asio::io_context &io;
    Socket socket{io};

    boost::asio::awaitable<bool>
    send_message(const std::string &message) const
    {
        try
        {
            co_await boost::asio::async_write(socket.socket, boost::asio::buffer(message), boost::asio::use_awaitable);
            co_return true;
        }
        catch (const boost::system::errro_code &ec)
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
            std::array<char, 1024> read_buffer;

            size_t total_bytes_read = boost::asio::async_read_until(socket.socket, boost::asio::buffer(read_buffer), "\nend", boost::asio::use_awaitable);

            std::cout.write(read_buffer.data(), total_bytes_read);

            co_await read_message_and_display();
        }
        catch (const boost::system::errro_code &ec)
        {
            std::cout << ec.message() << std::endl;
            co_return false;
        }
    }
};