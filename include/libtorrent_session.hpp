#ifndef CARACAL_LIBTORRENT_SESSION_HPP
#define CARACAL_LIBTORRENT_SESSION_HPP

#include <libtorrent/session.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert.hpp>
#include <string>
#include <vector>
#include <unordered_map>

namespace caracal {

    class LibTorrentSession {
    public:
        static LibTorrentSession& instance();
        void add_magnet(const std::string& magnet_uri);
        void set_storage_dir(const std::string& path);
        std::vector<lt::alert*> get_session_alerts();
        void add_handle(const std::string& key, const lt::torrent_handle& handle);
        lt::torrent_handle get_handle(const std::string& key);
    private:
        LibTorrentSession();
        ~LibTorrentSession();

        lt::session session_;
        std::string storage_dir_;
        std::unordered_map<std::string, lt::torrent_handle> handle_map_;
    };

} // floppa

#endif //CARACAL_CLIENT_SESSION_HPP
