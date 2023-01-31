#include <iostream>
#include <thread>
#include <chrono>
#include "../include/caracal.h"

int main() try {
    set_storage_dir(".");
    // some random fitgirl repack
    const char* magnet_uri = "magnet:?xt=urn:btih:6C2144689E395B6601CBCE55CEBBCBEEFE3B829B&dn=Mario+Kart+8+Deluxe+%28v1.7.1+%2B+Yuzu+Emu+for+PC%2C+MULTi9%29+%5BFitGirl+FIXED+Repack%5D&tr=udp%3A%2F%2F46.148.18.250%3A2710&tr=udp%3A%2F%2Fopentor.org%3A2710&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337%2Fannounce&tr=http%3A%2F%2Ftracker.dler.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2730%2Fannounce&tr=udp%3A%2F%2F9.rarbg.to%3A2770%2Fannounce&tr=udp%3A%2F%2Ftracker.pirateparty.gr%3A6969%2Fannounce&tr=http%3A%2F%2Fretracker.local%2Fannounce&tr=http%3A%2F%2Fretracker.ip.ncnet.ru%2Fannounce&tr=udp%3A%2F%2Fexodus.desync.com%3A6969%2Fannounce&tr=udp%3A%2F%2Fipv4.tracker.harry.lu%3A80%2Fannounce&tr=udp%3A%2F%2Fopen.stealth.si%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.zer0day.to%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969%2Fannounce&tr=udp%3A%2F%2Fcoppersurfer.tk%3A6969%2Fannounce";
    add_magnet(magnet_uri);
    
    while (true) {
        int size;
        alert* alerts = get_alerts(&size);

        free_alerts(alerts);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
} catch (std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
}