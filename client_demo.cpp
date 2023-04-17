#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "libtorrent/session.hpp"
#include "libtorrent/session_params.hpp"
#include "libtorrent/settings_pack.hpp"
#include "libtorrent/kademlia/dht_state.hpp"

int main(int argc, char *argv[])
{
    // initialize session
    std::cout << "Initializing session..." << std::endl;
    lt::settings_pack pack;
    pack.set_int(lt::settings_pack::alert_mask,
                      lt::alert_category::error
                    | lt::alert_category::storage
                    | lt::alert_category::dht
                    | lt::alert_category::status);
    lt::session session(pack);

    // bootstrap the DHT
    std::cout << "Bootstrapping the DHT..." << std::endl;
    session.add_dht_node(std::pair<std::string, int>("router.utorrent.com", 6881));
    int i = 0;
    while (i < 30)
    {
        std::vector<lt::udp::endpoint> dht_nodes = session.session_state().dht_state.nodes;
        if (dht_nodes.size() == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else {
            std::cout << "Successfully bootstrapped the DHT\nNodes:" << std::endl;
            for (auto node : dht_nodes)
            {
                std::cout << "\t" << node.address() << ":" << node.port() << std::endl;
            }
            break;
        }
        i++;
    }
    if (session.session_state().dht_state.nodes.size() == 0) {
        std::cout << "Unable to bootstrap DHT after 30 seconds of trying.\nExiting..." << std::endl;
        return EXIT_FAILURE;
    }

    

    

    return EXIT_SUCCESS;
}