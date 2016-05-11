#include "FileDownloader.h"

int cb_body(http_parser*, const char *at, size_t length)
{
    std::cout.write(at, length);
}

FileDownloader::FileDownloader()
 :resolve{ioservice}
 ,tcp_socket{ioservice}
 ,parser_settings{0}
{
    http_parser_init(&parser, HTTP_RESPONSE);
    parser_settings.on_body = cb_body;
}

FileDownloader::~FileDownloader()
{
}
