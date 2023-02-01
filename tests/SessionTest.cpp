#include <iostream>
#include <thread>
#include <chrono>
#include "../include/caracal.h"

int main() try {
    set_storage_dir(".");
    // some random fitgirl repack
    const char* magnet_uri = "magnet:?xt=urn:btih:5BBA656027C7FCF5A1417ACA626D6A4984B09EB4&dn=Avatar+The+Way+of+Water+2022+V3+HDTS+1080p+x264+AAC&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337%2Fannounce&tr=udp%3A%2F%2Fexodus.desync.com%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2970%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.tiny-vps.com%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337%2Fannounce&tr=udp%3A%2F%2Fopentor.org%3A2710%2Fannounce&tr=udp%3A%2F%2Ftracker.torrent.eu.org%3A451%2Fannounce&tr=udp%3A%2F%2Fexplodie.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.moeking.me%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.cyberia.is%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2980%2Fannounce&tr=udp%3A%2F%2F9.rarbg.to%3A2940%2Fannounce&tr=udp%3A%2F%2Ftracker.uw0.xyz%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337%2Fannounce&tr=http%3A%2F%2Ftracker.openbittorrent.com%3A80%2Fannounce&tr=udp%3A%2F%2Fopentracker.i2p.rocks%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969%2Fannounce&tr=udp%3A%2F%2Fcoppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.zer0day.to%3A1337%2Fannounce";
    add_magnet(magnet_uri);

    int time_limit = 20;
    auto start_time = std::chrono::steady_clock::now();
    
    while (true) {
        int size;
        alert* alerts = get_alerts(&size);

        for (int i = 0; i < size; i++) {
            if (alerts[i].type == TORRENT_STATE &&
                strcmp(alerts[i].torrent_state.state, "downloading") &&
                alerts[i].torrent_state.total_done >= 1000) {
                return EXIT_SUCCESS;
            }
            auto current_time = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() >= time_limit) {
                return EXIT_FAILURE;
            }
        }

        free_alerts(alerts);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
} catch (std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return EXIT_FAILURE;
}