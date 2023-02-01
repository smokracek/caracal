#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include "include/caracal.h"

enum class Command {
    POST,
    VIEW,
    EXIT,
    RUN_TORRENT,
    UNKNOWN
};

void post(std::vector<std::string> params) {
    
}

void view(std::vector<std::string> params) {

}

void run_torrent(const char* magnet_link) {
    std::cout << "Starting torrent" << std::endl;
    add_magnet(magnet_link);
    while (true) {
        int size;
        alert* alerts = get_alerts(&size);
        
        for (int i = 0; i < size; i++) {
            switch(alerts[i].type) {
                case TORRENT_STATE:
                    std::cout << '\r' << alerts[i].torrent_state.state << ' '
                          << (alerts[i].torrent_state.download_payload_rate / 1000) << " kB/s "
                          << (alerts[i].torrent_state.total_done / 1000) << " kB ("
                          << (alerts[i].torrent_state.progress_ppm / 10000) << "%) downloaded ("
                          << alerts[i].torrent_state.num_peers << " peers)\x1b[K";
                    std::cout.flush();

                    // TODO figure out why state == "finished" very beginning of torrent
                    // Probably need to research torrent state lifecycles
                    if ((strcmp(alerts[i].torrent_state.state, "finished") &&
                        alerts[i].torrent_state.progress_ppm == 1000000)) {
                        goto done;
                    }
                    break;
                default:
                    break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        free_alerts(alerts);
    }
    done:
    std::cout << "Torrent finished" << std::endl;
}

Command get_command(std::string word) {
    if (word == "post") return Command::POST;
    if (word == "view") return Command::POST;
    if (word == "exit") return Command::EXIT;
    if (word == "torrent") return Command::RUN_TORRENT;
    else return Command::UNKNOWN;
}

std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}

int main_loop() {
    std::cout << "Welcome to the Caracal demo client." << std::endl;

    for (;;) {
        std::cout << "caracal> ";
        std::string input;
        std::getline(std::cin, input);
        for (auto &c : input) c = std::tolower(c);
        std::vector<std::string> split_input = split(input);
        
        Command command = get_command(split_input.front());
        std::vector<std::string> params(split_input.begin() + 1, split_input.end());

        std::string magnet_link = "magnet:?xt=urn:btih:5BBA656027C7FCF5A1417ACA626D6A4984B09EB4&dn=Avatar+The+Way+of+Water+2022+V3+HDTS+1080p+x264+AAC&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337%2Fannounce&tr=udp%3A%2F%2Fexodus.desync.com%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2970%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.tiny-vps.com%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337%2Fannounce&tr=udp%3A%2F%2Fopentor.org%3A2710%2Fannounce&tr=udp%3A%2F%2Ftracker.torrent.eu.org%3A451%2Fannounce&tr=udp%3A%2F%2Fexplodie.org%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.moeking.me%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.cyberia.is%3A6969%2Fannounce&tr=udp%3A%2F%2F9.rarbg.me%3A2980%2Fannounce&tr=udp%3A%2F%2F9.rarbg.to%3A2940%2Fannounce&tr=udp%3A%2F%2Ftracker.uw0.xyz%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337%2Fannounce&tr=http%3A%2F%2Ftracker.openbittorrent.com%3A80%2Fannounce&tr=udp%3A%2F%2Fopentracker.i2p.rocks%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337%2Fannounce&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969%2Fannounce&tr=udp%3A%2F%2Fcoppersurfer.tk%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.zer0day.to%3A1337%2Fannounce";

        switch (command) {
            case Command::POST:
                post(params);
                break;
            case Command::VIEW:
                view(params);
                break;
            case Command::EXIT:
                return 0;
                break;
            case Command::UNKNOWN:
                std::cout << "Unknown command." << std::endl;
                break;
            case Command::RUN_TORRENT:
                run_torrent(magnet_link.c_str());
                break;
            default:
                break;
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    int exit_code = main_loop();
    if (exit_code == 0) {
        std::cout << "Exited successfully" << std::endl;
        return EXIT_SUCCESS;
    }
    std::cout << "Exited with error code " << exit_code << std::endl;
    return EXIT_FAILURE;
}