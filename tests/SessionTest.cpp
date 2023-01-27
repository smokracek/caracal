#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include "../include/caracal.hpp"

int main() try {
    // some random fitgirl repack
    std::string magnet_uri = "magnet:?xt=urn:btih:42FA7116F32802DC02F9173968F661B4876C5C48";
    caracal::addMagnet(magnet_uri);
    for(;;) {
        std::vector<std::string> alerts = caracal::get_alerts();
        for (std::string alert : alerts) {
            std::cout << alert << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
} catch (std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
}