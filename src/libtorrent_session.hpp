#ifndef CARACAL_LIBTORRENT_SESSION_HPP
#define CARACAL_LIBTORRENT_SESSION_HPP

#include <libtorrent/session.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/file_storage.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/sha1.hpp>
#include <string>
#include <vector>
#include <optional>

class LibTorrentSession
{
public:
    static LibTorrentSession &instance();
    lt::torrent_handle add_magnet(const std::string &magnet_uri);
    void set_post_storage_dir(const std::string &path);
    std::vector<lt::alert *> get_session_alerts();
    std::optional<lt::torrent_status> get_torrent_status(lt::torrent_handle handle);
    void create_torrent_file(const std::string &path);
    void set_dht_bootstrap_nodes(std::vector<std::pair<std::string, int>> nodes);
    lt::sha1_hash post_to_dht(lt::entry metadata);
    void query_dht(char *key);
    lt::torrent_handle seed_torrent(const std::string &path);
    void set_username(const std::string username);
    std::string get_username();

private:
    LibTorrentSession();
    ~LibTorrentSession();

    lt::session session_;
    std::string post_storage_dir_;
    std::string dht_bootstrap_nodes_;
    std::string username_;
};

#endif // CARACAL_CLIENT_SESSION_HPP
