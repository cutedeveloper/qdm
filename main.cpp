#include <iostream>
#include <http_parser.h>
#include "FileDownloader.h"

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        printf("usage: qdm [file to download] [output]\n");
        return -1;
    }
    FileDownloader file_downloader;
    file_downloader.set_download_filename(std::string(argv[1]));
    file_downloader.set_output_filename(std::string(argv[2]));
    file_downloader.start_download();
    return 0;
}
