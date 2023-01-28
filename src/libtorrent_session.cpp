#include "../include/libtorrent_session.hpp"
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/settings_pack.hpp>

namespace floppa {

    LibTorrentSession& LibTorrentSession::instance() {
        static LibTorrentSession* session = new LibTorrentSession();
        return *session;
    }

    LibTorrentSession::LibTorrentSession() {
        lt::settings_pack settings;
        settings.set_int(
            lt::settings_pack::alert_mask, 
            lt::alert_category::status 
            | lt::alert_category::error
            | lt::alert_category::storage);
        settings.set_str(lt::settings_pack::listen_interfaces, "0.0.0.0:6881,[::1]:6881");
        lt::session session_(settings);
    };

    LibTorrentSession::~LibTorrentSession() {
        session_.~session();
    }

    void LibTorrentSession::addMagnet(const std::string& magnet_uri) {
        lt::add_torrent_params params = lt::parse_magnet_uri(magnet_uri);
        params.save_path = storage_dir_;
        session_.add_torrent(std::move(params));
    }

    void LibTorrentSession::pauseSession() {
        session_.pause();
    }

    void LibTorrentSession::resumeSession() {
        if (session_.is_paused()) {
            session_.resume();
            std::vector<lt::torrent_handle> const torrents = session_.get_torrents();
            for (auto torrent: torrents) {
                torrent.resume();
            }
        }
    }

    void LibTorrentSession::set_storage_dir(const std::string& path) {
        LibTorrentSession::storage_dir_ = path;
    }

    std::vector<std::string> LibTorrentSession::get_alerts() {
        std::vector<lt::alert*> alerts;
        session_.pop_alerts(&alerts);
        std::vector<std::string> ret;
        for (auto alert : alerts) {
            ret.push_back("Alert: " + std::string(alert->what()) + " " + alert->message());
        }
        return ret;
    }
} // floppa