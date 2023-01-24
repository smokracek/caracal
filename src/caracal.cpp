#include <iostream>
#include "../include/client_session.hpp"
#include "../include/caracal.hpp"

namespace caracal {

    void pause() {
        floppa::ClientSession::instance().pauseSession();
    }

    void resume() {
        floppa::ClientSession::instance().resumeSession();
    }

    void stop() {
        floppa::ClientSession::instance().stopSession();
    }

    int set_storage_dir(const std::string& path) {
        floppa::ClientSession::instance().set_storage_dir(path);
        return 1;
    }

    void addMagnet(const std::string& magnet_uri) {
        floppa::ClientSession::instance().addMagnet(magnet_uri);
    }

}
