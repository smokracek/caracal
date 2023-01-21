//
// Created by Sam Mokracek on 1/19/23.
//

#include "floppa_session.hpp"
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_handle.hpp>

namespace floppa {

    Session &Session::instance() {
        static Session session;
        return session;
    }

    Session::Session() = default;

    Session::~Session() {
        session_.~session();
    }

    void Session::addMagnet(const std::string& magnet_uri) {
        lt::add_torrent_params params = lt::parse_magnet_uri(magnet_uri);
        params.save_path = Session::storage_dir_;
        session_.async_add_torrent(params);
    }

    void Session::pauseSession() {
        session_.pause();
    }

    void Session::resumeSession() {
        if (session_.is_paused()) {
            session_.resume();
            std::vector<lt::torrent_handle> const torrents = session_.get_torrents();
            for (lt::torrent_handle const& torrent: torrents) {
                torrent.resume();
            }
        }
    }

    void Session::stopSession() {
        session_.abort();
    }

    void Session::set_storage_dir(const std::string& path) {
        Session::storage_dir_ = path;
    }
} // floppa