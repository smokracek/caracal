#include "libtorrent_session.hpp"
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/session_params.hpp>
#include <libtorrent/settings_pack.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/file_storage.hpp>
#include <libtorrent/create_torrent.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/hasher.hpp>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <stdexcept>

LibTorrentSession &LibTorrentSession::instance()
{
    static LibTorrentSession *session = new LibTorrentSession();
    return *session;
}

LibTorrentSession::LibTorrentSession()
{
    set_post_storage_dir(".");
    lt::session_params params;
    params.settings.set_int(
        lt::settings_pack::alert_mask,
        lt::alert_category::status |
            lt::alert_category::error |
            lt::alert_category::storage |
            lt::alert_category::piece_progress |
            lt::alert_category::dht |
            lt::alert_category::dht_operation);
    lt::session session_(params);
};

LibTorrentSession::~LibTorrentSession()
{
    session_.~session();
}

lt::torrent_handle LibTorrentSession::add_magnet(const std::string &magnet_uri)
{
    lt::add_torrent_params magnet = lt::parse_magnet_uri(magnet_uri);

    magnet.save_path = post_storage_dir_;
    lt::torrent_handle handle = session_.add_torrent(std::move(magnet));
    return handle;
}

void LibTorrentSession::set_post_storage_dir(const std::string &path)
{
    LibTorrentSession::post_storage_dir_ = path;
}

std::vector<lt::alert *> LibTorrentSession::get_session_alerts()
{
    session_.post_torrent_updates();
    std::vector<lt::alert *> lt_alerts;
    session_.pop_alerts(&lt_alerts);
    return lt_alerts;
}

void LibTorrentSession::create_torrent_file(const std::string &file_name)
{
#ifdef _WIN32
    std::string file_path(std::filesystem::current_path().string() + "\\" + file_name);
#else
    std::string file_path(std::filesystem::current_path().string() + "/" + file_name);
#endif

    if (!std::filesystem::exists(file_path))
    {
        throw std::ifstream::failure("Error: File not found on path " + file_path);
    }

    std::string outfile = file_path + ".torrent";
    std::string parent_dir = file_path.substr(0, file_path.find_last_of("/\\"));

    lt::file_storage fs;
    int piece_size = 0;
    lt::create_flags_t flags = {};
    lt::add_files(fs, file_path, flags);

    lt::create_torrent t(fs, piece_size, flags);

    try
    {
        lt::set_piece_hashes(t, parent_dir);
    }
    catch (const std::exception &e)
    {
        std::string what(e.what());
        throw std::runtime_error("Error setting piece hashes: " + what);
    }

    std::vector<char> torrent;
    lt::bencode(back_inserter(torrent), t.generate());
    std::fstream out;
    out.open(outfile.c_str(), std::ios_base::out | std::ios_base::binary);
    out.write(torrent.data(), int(torrent.size()));
}

void LibTorrentSession::set_dht_bootstrap_nodes(std::vector<std::pair<std::string, int>> nodes)
{
    for (auto node : nodes)
    {
        session_.add_dht_node(node);
    }

    int i = 0;
    while (i < 30)
    {
        std::vector<lt::udp::endpoint> dht_nodes = session_.session_state(lt::session_handle::save_dht_state).dht_state.nodes;
        if (dht_nodes.size() == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        else
        {
            std::cout << "Current DHT Nodes:" << std::endl;
            for (auto node : dht_nodes)
            {
                std::cout << node.address() << ":" << node.port() << std::endl;
            }
            return;
        }
        i++;
    }
    throw std::runtime_error("Failed to connect to any DHT nodes");
}

lt::sha1_hash LibTorrentSession::post_to_dht(lt::entry metadata)
{
    lt::sha1_hash hash;
    try
    {
        hash = session_.dht_put_item(metadata);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        throw std::runtime_error("Error posting to DHT");
    }
    return hash;
}

void LibTorrentSession::query_dht(char *key)
{
    lt::hasher hasher;
    hasher.update(key, sizeof(key));
    session_.dht_get_item(hasher.final());
}

lt::torrent_handle LibTorrentSession::seed_torrent(const std::string &path)
{
    lt::add_torrent_params params;
    params.save_path = "../" + post_storage_dir_;
    params.ti = std::make_shared<lt::torrent_info>(std::string(path + ".torrent"));
    lt::torrent_handle handle = session_.add_torrent(params);
    return handle;
}

void LibTorrentSession::set_username(const std::string username)
{
    username_ = username;
}

std::string LibTorrentSession::get_username()
{
    return username_;
}
