#include "client_session.hpp"
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_handle.hpp>

namespace floppa {

    ClientSession &ClientSession::instance() {
        static ClientSession session;
        return session;
    }

    ClientSession::ClientSession() = default;

    ClientSession::~ClientSession() {
        session_.~session();
    }

    void ClientSession::addMagnet(const std::string& magnet_uri) {
        lt::add_torrent_params params = lt::parse_magnet_uri(magnet_uri);
        params.save_path = ClientSession::storage_dir_;
        session_.async_add_torrent(params);
    }

    void ClientSession::pauseSession() {
        session_.pause();
    }

    void ClientSession::resumeSession() {
        if (session_.is_paused()) {
            session_.resume();
            std::vector<lt::torrent_handle> const torrents = session_.get_torrents();
            for (lt::torrent_handle const& torrent: torrents) {
                torrent.resume();
            }
        }
    }

    void ClientSession::stopSession() {
        session_.abort();
    }

    void ClientSession::set_storage_dir(const std::string& path) {
        ClientSession::storage_dir_ = path;
    }
} // floppa