#include <iostream>
#include <thread>
#include <chrono>
#include "../include/caracal.h"

int main() try {
    // some random fitgirl repack
    set_storage_dir(".");
    const char* magnet_uri = "magnet:?xt=urn:btih:42FA7116F32802DC02F9173968F661B4876C5C48";
    add_magnet(magnet_uri);
    
    for (;;) {
        struct alert* alerts;
        int size;
        alerts = get_alerts(&size);

        for (int i = 0; i < size; i++) {
            struct alert current = alerts[i];
            std::cout << current.type << ": " << current.message << std::endl;
        }

        free_alerts(alerts);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
} catch (std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
}