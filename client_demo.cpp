#include <iostream>

void post() {

}

void view() {

}

int main_loop() {
    std::cout << "Welcome to the Caracal demo client." << std::endl;

    for (;;) {
        std::string post_view;
        std::cout << "Post (p) or view posts (v) or quit (q)?: ";
        std::cin >> post_view;
        if (post_view == "p") {

        } else if (post_view == "v") {

        } else if (post_view == "q") {
            return 0;
        } else {
            std::cout << "Invalid option." << std::endl;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int exit_code = main_loop();
    if (exit_code == 0) {
        std::cout << "Exited successfully" << std::endl;
        return 0;
    } else {
        std::cout << "Exited with error code " << exit_code << std::endl;
        return 1;
    }
}