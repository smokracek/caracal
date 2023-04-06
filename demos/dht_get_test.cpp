#include <iostream>
#include <thread>
#include <chrono>
#include "../include/caracal.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Wrong number of args" << std::endl;
        return EXIT_FAILURE;
    }

    set_dht_bootstrap_nodes("router.utorrent.com:6881");
    get_user_post_magnets(argv[1]);

    dht_item_t *items = (dht_item_t *)malloc(10 * sizeof(dht_item_t));
    while (true)
    {
        items = check_dht_queries(argv[1]);
        printf("%c", 0x7);
        if (items[0] != nullptr)
            break;
    }

    for (int i = 0; i < 10; i++)
    {
        if (items[i] != nullptr)
        {
            std::cout << get_dht_item_magnet(items[i]) << std::endl;
        }
    }

    return EXIT_SUCCESS;
}