#include <iostream>
#include "../include/caracal.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Incorrect number of arguments." << std::endl;
        return EXIT_FAILURE;
    }

    if (set_dht_bootstrap_nodes("router.utorrent.com:6881"))
    {
        return EXIT_FAILURE;
    }

    set_username("sammoe13");

    char *hash = post_to_dht(argv[1]);

    std::cout << "Hash: " << hash << std::endl;

    return EXIT_SUCCESS;
}