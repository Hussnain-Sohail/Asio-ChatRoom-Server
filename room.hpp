#include <iostream>
#include <vector>
#include <coroutine>
#include <memory>
#include <boost/asio.hpp>
#include <openssl/rand.h>
#include "socket.hpp"
#include <mutex>

#ifndef ROOM
#define ROOM
using boost::asio::ip::tcp;
class Room
{
private:
    std::mutex mtx;
    std::vector<std::shared_ptr<Socket>> connected_sockets;

public:
    void add_socket(Socket &socket) noexcept
    {
        connected_sockets.emplace_back(std::make_shared<Socket>(std::move(socket)));
    }

    bool transmit_message(const std::string &msg, const std::string &sender_id)
    {
        try
        {
            for (std::shared_ptr<Socket> socket : connected_sockets)
            {
                {
                    std::unique_lock<std::mutex> lock{mtx};
                    if (socket->give_socket_id() != sender_id)
                        boost::asio::async_write(socket->give_socket(), boost::asio::buffer(msg), [socket](auto, auto) {});
                }
            }
            return true;
        }
        catch (const boost::system::error_code &ec)
        {
            std::cout << ec.message() << std::endl;
            return false;
        }
    }

    bool remove_socket(const std::string &socket_id) noexcept
    {
        if (socket_id.empty())
            return false;

        for (size_t i = 0; i < connected_sockets.size(); i++)
        {
            {
                std::unique_lock<std::mutex> lock{mtx};
                if (connected_sockets[i]->give_socket_id() == socket_id)
                {
                    connected_sockets.erase(connected_sockets.begin() + i);
                    return true;
                }
            }
        }

        return false;
    }

    Room() = default;
    ~Room() = default;

    Room(const Room &) = delete;
    Room &operator=(const Room &) = delete;

    Room(Room &&) = delete;
    Room &operator=(Room &&) = delete;
};
#endif;