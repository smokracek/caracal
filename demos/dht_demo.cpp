#include "../include/caracal.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Recieved wrong number of arguments" << std::endl;
        return EXIT_FAILURE;
    }

    const char *boostrap_nodes = argv[1];
    try
    {
        set_dht_bootstrap_nodes(boostrap_nodes);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    std::cout << "Set DHT bootstrap nodes successfully" << std::endl;

    const char *file_name = argv[2];
    post_bundle_t post;
    try
    {
        post = create_post(file_name);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    std::cout << "\nCreated post:\n"
              << "Title: " << std::string(get_post_title(post)) << ".torrent\n"
              << "Magnet Link: " << std::string(get_post_magnet(post)) << std::endl;

    return EXIT_SUCCESS;
}