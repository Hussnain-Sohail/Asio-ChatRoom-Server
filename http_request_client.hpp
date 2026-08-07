#include <iostream>
#include <sstream>
#include <optional>

#ifndef HTTP_REQUEST_CLIENT
#define HTTP_REQUEST_CLIENT

class HTTP_CLIENT
{
private:
    std::ostringstream os;
    std::istringstream is;

public:
    std::string format_request(const std::string &body, const std::string &host) noexcept
    {
        os << "POST /message HTTP/1.1\r\n"
           << "Host: " << host << "\r\n"
           << "Content-Type: text/plain\r\n"
           << "Content-Length: " << body.length() << "\r\n"
           << "\r\n"
           << body
           << "\nend";

        return os.str();
    }

    std::optional<std::string>
    parse_request(const std::string &http_request) noexcept
    {
        if (http_request.empty())
            return std::nullopt;

        size_t body_pos = http_request.find("\r\n\r\n");

        if (body_pos == std::string::npos)
            return std::nullopt;

        const std::string body = http_request.substr(body_pos + 4);

        if (body.empty())
            return std::nullopt;

        return body;
    }
};

#endif;