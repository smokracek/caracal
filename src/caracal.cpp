#include "client_session.hpp"
#include "caracal.hpp"

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

    void set_storage_dir(const std::string& path) {
        floppa::ClientSession::instance().set_storage_dir(path);
    }

    void addMagnet(const std::string& magnet_uri) {
        floppa::ClientSession::instance().addMagnet(magnet_uri);
    }

}
