#include <iostream>
#include "../include/caracal.hpp"

int main() {
    assert(caracal::set_storage_dir(".") == 1);
    return 0;
}