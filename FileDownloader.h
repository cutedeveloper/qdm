#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <string>
#include <boost/asio/buffer.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <http_parser.h>
#include <boost/network/uri.hpp>

class FileDownloader
{
public:
    FileDownloader();
    ~FileDownloader();
    void set_output_filename(const std::string& fn)
    {
        output_file_name = fn;
    }

    std::string get_output_filename() const
    {
        return output_file_name;
    }

    void set_download_filename(const std::string& fn)
    {
        download_file_name = fn;
    }

    std::string get_download_filename() const
    {
        return download_file_name;
    }

    void read_handler(const boost::system::error_code& ec, std::size_t byte_trans)
    {
        if (!ec)
        {
            http_parser_execute(&parser, &	parser_settings, bytes.data(), byte_trans);
            tcp_socket.async_read_some(boost::asio::buffer(bytes),
                                       boost::bind(&FileDownloader::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
    }

    void connect_handler(const boost::system::error_code& ec)
    {
        if (!ec)
        {
            boost::network::uri::uri uri(download_file_name);
            if (!uri.is_valid())
            {
                tcp_socket.close();
                return;
            }
            std::string req =
                            "GET /" + uri.path() + " HTTP/1.1\r\nHost: " + uri.host() + "\r\n\r\n";
            write(tcp_socket, boost::asio::buffer(req));
            tcp_socket.async_read_some(boost::asio::buffer(bytes),
                                       boost::bind(&FileDownloader::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
    }

    void resolve_handler(const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator it)
    {
        if (!ec)
            tcp_socket.async_connect(*it, boost::bind(&FileDownloader::connect_handler, this, boost::asio::placeholders::error));
    }

    void start_download()
    {
        boost::network::uri::uri uri(download_file_name);
        if(!uri.is_valid())
        {
            printf("Requested uri is not valid.\n");
            return;
        }
        boost::asio::ip::tcp::resolver::query query{uri.host(), "80"};
        resolve.async_resolve(query, boost::bind(&FileDownloader::resolve_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
        ioservice.run();
    }

    void stop_download();
    void pause_download();

private:
    std::string output_file_name;		/// Filename to download
    std::string download_file_name;			/// Output filename
    http_parser parser;
    http_parser_settings parser_settings;
    std::array<char, 4096> bytes;
    boost::asio::io_service ioservice;
    boost::asio::ip::tcp::socket tcp_socket;
    boost::asio::ip::tcp::resolver resolve;

};

#endif // FILEDOWNLOADER_H
