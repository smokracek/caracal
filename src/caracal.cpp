#include "../include/libtorrent_session.hpp"
#include "../include/caracal.hpp"

namespace caracal {

    void pause() {
        floppa::LibTorrentSession::instance().pauseSession();
    }

    void resume() {
        floppa::LibTorrentSession::instance().resumeSession();
    }

    void set_storage_dir(const std::string& path) {
        floppa::LibTorrentSession::instance().set_storage_dir(path);
    }

    void addMagnet(const std::string& magnet_uri) {
        floppa::LibTorrentSession::instance().addMagnet(magnet_uri);
    }

    std::vector<std::string> get_alerts() {
        return floppa::LibTorrentSession::instance().get_alerts();
    }
}
