#ifndef CARACAL_LIBTORRENT_SESSION_HPP
#define CARACAL_LIBTORRENT_SESSION_HPP

#include <libtorrent/session.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/file_storage.hpp>
#include <string>
#include <vector>
#include <optional>

class LibTorrentSession
{
public:
    static LibTorrentSession &instance();
    lt::torrent_handle add_magnet(const std::string &magnet_uri);
    void set_download_storage_dir(const std::string &path);
    void set_post_storage_dir(const std::string &path);
    std::vector<lt::alert *> get_session_alerts();
    std::optional<lt::torrent_status> get_torrent_status(lt::torrent_handle handle);
    void create_torrent_file(const std::string &path);

private:
    LibTorrentSession();
    ~LibTorrentSession();

    lt::session session_;
    std::string download_storage_dir_;
    std::string post_storage_dir_;
};

#endif // CARACAL_CLIENT_SESSION_HPP
