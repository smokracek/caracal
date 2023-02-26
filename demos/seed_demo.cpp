#include "../include/caracal.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Error: incorrect number of arguments." << std::endl;
        return EXIT_FAILURE;
    }

    const char *magnet_link = argv[1];

    torrent_handle_t seeded_torrent = download_post(magnet_link);

    while (true)
    {
        download_status_t status = get_download_status(seeded_torrent);
        std::cout << "Status type: " << get_status_type_string(status) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return EXIT_SUCCESS;
}