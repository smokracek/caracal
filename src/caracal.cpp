//
// Created by Sam Mokracek on 1/19/23.
//

#include "floppa_session.hpp"
#include "caracal.hpp"

namespace caracal {

    void pause() {
        floppa::Session::instance().pauseSession();
    }

    void resume() {
        floppa::Session::instance().resumeSession();
    }

    void stop() {
        floppa::Session::instance().stopSession();
    }

    void set_storage_dir(const std::string& path) {
        floppa::Session::instance().set_storage_dir(path);
    }

    void addMagnet(const std::string& magnet_uri) {
        floppa::Session::instance().addMagnet(magnet_uri);
    }

}
