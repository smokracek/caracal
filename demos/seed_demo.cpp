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

    if (set_dht_bootstrap_nodes("router.utorrent.com:6881,router.bittorent.com:6881") == EXIT_FAILURE)
    {
        std::cout << "Failed to set bootstrap nodes" << std::endl;
        return EXIT_FAILURE;
    }

    const char *file_path = argv[1];
    post_bundle_t bundle = create_post(file_path);
    torrent_handle_t handle = seed_file(bundle);
    std::cout << "Magnet: " << get_post_magnet(bundle) << std::endl;

    while (true)
    {
        handle_alerts();
        torrent_status_t status = get_torrent_status(handle);
        std::cout << "\r" << get_status_type_string(status);
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
