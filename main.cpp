#include <iostream>
#include <http_parser.h>
#include "FileDownloader.h"

int main()
{
    FileDownloader file_downloader;
    file_downloader.set_download_filename("http://www.example.com");
    file_downloader.start_download();
}
