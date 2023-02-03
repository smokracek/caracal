#ifndef CARACAL_LIBTORRENT_SESSION_HPP
#define CARACAL_LIBTORRENT_SESSION_HPP

#include <libtorrent/session.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace caracal
{

    class LibTorrentSession
    {
    public:
        static LibTorrentSession &instance();
        void add_magnet(const std::string &magnet_uri);
        void set_storage_dir(const std::string &path);
        std::vector<lt::alert *> get_session_alerts();
        std::optional<lt::torrent_status> get_torrent_status(lt::torrent_handle handle);

    private:
        LibTorrentSession();
        ~LibTorrentSession();

        lt::session session_;
        std::string storage_dir_;
    };

} // caracal

#endif // CARACAL_CLIENT_SESSION_HPP
