#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include "include/caracal.h"

enum class Command
{
    POST,
    VIEW,
    EXIT,
    RUN_TORRENT,
    UNKNOWN
};

void post(std::vector<std::string> params)
{
    post_bundle_t post = {};
    if (params.size() != 2)
    {
        std::cout << "Please provide a -f flag followed by a file path." << std::endl;
        return;
    }
    if (params.front() == "-f")
    {
        post = create_post(params.back().c_str());
        std::cout << "Title: " << get_post_title(post) << std::endl;
        std::cout << "Magnet: " << get_post_magnet(post) << std::endl;
        return;
    }
    else
    {
        std::cout << "Got unknown parameters! Exiting to main." << std::endl;
        return;
    }

    // TODO, do something with the post struct
}

void view(std::vector<std::string> params)
{
}

void run_torrent(std::vector<std::string> params)
{
    torrent_handle_t handle = {};
    if (params.size() != 2)
    {
        std::cout << "Please provide either a -l or -f flag followed by a magnet link or file path, respectively." << std::endl;
        return;
    }
    if (params.front() == "-l")
    {
        handle = add_magnet(params.back().c_str());
    }
    else if (params.front() == "-f")
    {
        std::ifstream in_file(params.back());
        if (!in_file.is_open())
        {
            std::cout << "ERROR: could not open file!" << std::endl;
            return;
        }
        std::string magnet_link;
        std::getline(in_file, magnet_link);
        handle = add_magnet(magnet_link.c_str());
        in_file.close();
    }
    else
    {
        std::cout << "Got unknown parameters! Exiting to main." << std::endl;
        return;
    }
    std::cout << "Starting torrent" << std::endl;
    std::cout << get_torrent_name(handle) << std::endl;
    while (true)
    {
        handle_alerts();
        torrent_status_t status = get_torrent_status(handle);
        if (get_status_type(status) == FINISHED)
        {
            break;
        }
        std::string status_type = std::string(get_status_type_string(status));

        std::cout << '\r' << status_type
                  << ": " << get_num_peers(status) << " peers, "
                  << ((float)get_progress_ppm(status) / 10000) << "% finished";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "DONE" << std::endl;
    return;
}

Command get_command(std::string word)
{
    if (word == "post")
        return Command::POST;
    if (word == "view")
        return Command::POST;
    if (word == "exit")
        return Command::EXIT;
    if (word == "torrent")
        return Command::RUN_TORRENT;
    else
        return Command::UNKNOWN;
}

std::vector<std::string> split(const std::string &s)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, ' '))
    {
        tokens.push_back(token);
    }
    return tokens;
}

int main_loop()
{
    std::cout << "Welcome to the Caracal demo client." << std::endl;

    for (;;)
    {
        std::cout << "caracal> ";
        std::string input;
        std::getline(std::cin, input);
        for (auto &c : input)
        {
            c = std::tolower(c);
        }
        std::vector<std::string> split_input = split(input);

        Command command = get_command(split_input.front());
        std::vector<std::string> params(split_input.begin() + 1, split_input.end());

        switch (command)
        {
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
            run_torrent(params);
            break;
        default:
            break;
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    int exit_code = main_loop();
    if (exit_code == 0)
    {
        std::cout << "Exited successfully" << std::endl;
        return EXIT_SUCCESS;
    }
    std::cout << "Exited with error code " << exit_code << std::endl;
    return EXIT_FAILURE;
}