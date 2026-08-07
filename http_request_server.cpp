#include <iostream>
#include <optional>
#include <sstream>

class HTTP_SERVER
{
private:
    std::ostringstream os;
    std::istringstream is;

public:
    std::string format_request(const std::string &status, const std::string &body) noexcept
    {
        if (status.empty())
            return "";

        os << "HTTP/1.1 " << status << "\r\n"
           << "Content-Type: text/plain\r\n"
           << "Content-Length: " << body.length() << "\r\n"
           << "\r\n"
           << body;

        return os.str();
    }

    std::optional<std::string>
    parse_request(const std::string &http_request) noexcept
    {
        if (http_request.empty())
            return std::nullopt;
    }
};