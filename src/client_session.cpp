#include "../include/client_session.hpp"
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/settings_pack.hpp>

namespace floppa {

    ClientSession& ClientSession::instance() {
        static ClientSession* session = new ClientSession();
        return *session;
    }

    ClientSession::ClientSession() {
        lt::settings_pack settings;
        settings.set_int(
            lt::settings_pack::alert_mask, 
            lt::alert_category::status | lt::alert_category::error);
        settings.set_str(lt::settings_pack::listen_interfaces, "0.0.0.0:6981");
        lt::session session_(settings);
    };

    ClientSession::~ClientSession() {
        session_.~session();
    }

    void ClientSession::addMagnet(const std::string& magnet_uri) {
        lt::add_torrent_params params = lt::parse_magnet_uri(magnet_uri);
        params.save_path = storage_dir_;
        session_.async_add_torrent(std::move(params));
    }

    void ClientSession::pauseSession() {
        session_.pause();
    }

    void ClientSession::resumeSession() {
        if (session_.is_paused()) {
            session_.resume();
            std::vector<lt::torrent_handle> const torrents = session_.get_torrents();
            for (auto torrent: torrents) {
                torrent.resume();
            }
        }
    }

    void ClientSession::set_storage_dir(const std::string& path) {
        ClientSession::storage_dir_ = path;
    }

    std::vector<std::string> ClientSession::get_alerts() {
        std::vector<lt::alert*> alerts;
        session_.pop_alerts(&alerts);
        std::vector<std::string> ret;
        for (auto alert : alerts) {
            ret.push_back("Alert: " + std::string(alert->what()) + " " + alert->message());
        }
        return ret;
    }
} // floppa