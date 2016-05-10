#include <iostream>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <http_parser.h>

using namespace boost::asio::ip;
using namespace boost::asio;

io_service ioservice;
tcp::resolver resolve{ioservice};
tcp::socket tcp_socket{ioservice};
std::array<char, 4096> bytes;
http_parser parser;
http_parser_settings settings{0};

void read_handler(const boost::system::error_code& ec, std::size_t byte_trans)
{
    if (!ec)
    {
        http_parser_execute(&parser, &settings, bytes.data(), byte_trans);
        tcp_socket.async_read_some(buffer(bytes), read_handler);
    }
}

void connect_handler(const boost::system::error_code& ec)
{
    if (!ec)
    {
        std::string req =
                "GET / HTTP/1.1\r\nHost: theboostcpplibraries.com\r\n\r\n";
        write(tcp_socket, buffer(req));
        tcp_socket.async_read_some(buffer(bytes), read_handler);
    }
}

void resolve_handler(const boost::system::error_code& ec, tcp::resolver::iterator it)
{
    if (!ec)
        tcp_socket.async_connect(*it, connect_handler);
}
int cb_body(http_parser*, const char *at, size_t length)
{
    std::cout.write(at, length);
}

int main()
{
    http_parser_init(&parser, HTTP_RESPONSE);

    settings.on_body = cb_body;
    tcp::resolver::query query{"theboostcpplibraries.com", "80"};
    resolve.async_resolve(query, resolve_handler);
    ioservice.run();
}
