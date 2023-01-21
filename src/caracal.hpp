//
// Created by Sam Mokracek on 1/19/23.
//

#ifndef CARACAL_CARACAL_H
#define CARACAL_CARACAL_H

namespace caracal {

    void pause();
    void resume();
    void stop();

    void set_storage_dir(const std::string& path);
    void addMagnet(const std::string& magnet_uri);
} // caracal

#endif //CARACAL_CARACAL_H
