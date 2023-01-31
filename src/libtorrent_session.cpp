#include "../include/libtorrent_session.hpp"
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/session_params.hpp>
#include <libtorrent/settings_pack.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <fstream>

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

    void LibTorrentSession::add_magnet(const std::string& magnet_uri) {
        std::ifstream ifs(".resume_file", std::ios_base::binary);
        ifs.unsetf(std::ios_base::skipws);
        std::vector<char> buf{
            std::istream_iterator<char>(ifs), 
            std::istream_iterator<char>()
        };

        lt::add_torrent_params magnet = lt::parse_magnet_uri(magnet_uri);
        if (!buf.empty()) {
            lt::add_torrent_params atp = lt::read_resume_data(buf);
            if (atp.info_hashes == magnet.info_hashes) {
                magnet = std::move(atp);
            }
        }

        magnet.save_path = storage_dir_;
        session_.async_add_torrent(std::move(magnet));
    }

    void LibTorrentSession::set_storage_dir(const std::string& path) {
        LibTorrentSession::storage_dir_ = path;
    }

    std::vector<lt::alert*> LibTorrentSession::get_session_alerts() {
        session_.post_torrent_updates();
        std::vector<lt::alert*> lt_alerts;
        session_.pop_alerts(&lt_alerts);
        return lt_alerts;
    }

    void LibTorrentSession::add_handle(const std::string& key, const lt::torrent_handle& handle) {
        handle_map_[key] = handle;
    }

    lt::torrent_handle LibTorrentSession::get_handle(const std::string& key) {
        return handle_map_[key];
    }
} // caracal