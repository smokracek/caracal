#include "../include/libtorrent_session.hpp"
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/session_params.hpp>
#include <libtorrent/settings_pack.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <iostream>

namespace caracal {

    LibTorrentSession& LibTorrentSession::instance() {
        static LibTorrentSession* session = new LibTorrentSession();
        return *session;
    }

    LibTorrentSession::LibTorrentSession() {
        set_storage_dir(".");
        lt::session_params params;
        params.settings.set_int(
            lt::settings_pack::alert_mask, 
            lt::alert_category::status 
            | lt::alert_category::error
            | lt::alert_category::storage);
        lt::session session_(params);
    };

    LibTorrentSession::~LibTorrentSession() {
        session_.~session();
    }

    void LibTorrentSession::addMagnet(const std::string& magnet_uri) {
        lt::add_torrent_params params = lt::parse_magnet_uri(magnet_uri);
        params.save_path = storage_dir_;
        session_.async_add_torrent(std::move(params));
    }

    void LibTorrentSession::set_storage_dir(const std::string& path) {
        LibTorrentSession::storage_dir_ = path;
    }

    std::vector<lt::alert*> LibTorrentSession::get_session_alerts() {
        std::vector<lt::alert*> lt_alerts;
        session_.pop_alerts(&lt_alerts);
        return lt_alerts;
    }
} // caracal