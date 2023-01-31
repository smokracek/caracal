#include <iostream>
#include <thread>
#include <chrono>
#include "../include/caracal.h"

int main() try {
    // some random fitgirl repack
    set_storage_dir(".");
    const char* magnet_uri = "magnet:?xt=urn:btih:42FA7116F32802DC02F9173968F661B4876C5C48";
    add_magnet(magnet_uri);
    
    while (true) {
        int size;
        alert* alerts = get_alerts(&size);

        for (int i = 0; i < size; i++) {
            alert current = alerts[i];
            if (strcmp(current.type, "state_update")) {
                std::cout << current.type << ": " << current.message << std::endl;
            }
            if (current.has_stats == 1) {
                torrent_stats stats = current.stats;
                std::cout << '\r' << stats.state << ' '
                          << (stats.download_rate / 1000) << " kB/s "
                          << (stats.total_done / 1000) << " kB ("
                          << (stats.progress_ppm / 10000) << "%) downloaded ("
                          << stats.num_peers << " peers)\x1b[K";
                std::cout.flush();
            }
        }

        free_alerts(alerts);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
} catch (std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
}