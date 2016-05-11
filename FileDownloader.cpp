#include "FileDownloader.h"

FileDownloader::FileDownloader()
 :resolve{ioservice}
 ,tcp_socket{ioservice}
 ,parser_settings{0}
{
    http_parser_init(&parser, HTTP_RESPONSE);
    parser_settings.on_body = &FileDownloader::cb_body;
    parser.data = this;
    parser_settings.on_headers_complete = &FileDownloader::cb_headers_complete;
}

FileDownloader::~FileDownloader()
{
}
