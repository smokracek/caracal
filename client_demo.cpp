#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

enum class Command {
    POST,
    VIEW,
    EXIT,
    UNKNOWN
};

void post(std::vector<std::string> params) {
    
}

void view(std::vector<std::string> params) {

}

Command get_command(std::string word) {
    if (word == "post") return Command::POST;
    if (word == "view") return Command::POST;
    if (word == "exit") return Command::EXIT;
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