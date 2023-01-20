cmake_minimum_required(VERSION 3.24)
project(Caracal)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)
find_library(LIBTORRENT libtorrent-rasterbar.dylib)

add_library(Caracal src/caracal.hpp src/caracal.cpp src/Session.cpp src/Session.h)
