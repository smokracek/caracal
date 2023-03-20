#include "../include/caracal.h"
#include "libtorrent_session.hpp"
#include "torrent_pool.hpp"
#include "torrent_status.h"
#include "torrent_handle.h"
#include "post_bundle.h"
#include "magnet_pool.hpp"
#include "dht_item.h"
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/write_resume_data.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/create_torrent.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/hasher.hpp>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream>
#include <utility>

void init_session(void)
{
    LibTorrentSession::instance();
}

void set_post_storage_dir(const char *path)
{
    LibTorrentSession::instance().set_post_storage_dir(path);
}

void set_username(const char *username)
{
    LibTorrentSession::instance().set_username(std::string(username));
}

torrent_handle_t download_post(const char *magnet_uri)
{
    lt::torrent_handle lt_handle = LibTorrentSession::instance().add_magnet(magnet_uri);
    TorrentPool::instance().add_torrent(lt_handle);
    MagnetPool::instance().add_magnet(lt_handle.status().name, std::string(magnet_uri));
    torrent_handle_t handle = (torrent_handle_t)malloc(sizeof(_torrent_handle_instance_t));
    handle->id = lt_handle.id();
    strncpy(handle->name, lt_handle.status().name.c_str(), sizeof(handle->name));
    return handle;
}

status_type_t state(lt::torrent_status::state_t s)
{
    switch (s)
    {
    case lt::torrent_status::checking_files:
        return CHECKING_FILES;
    case lt::torrent_status::downloading_metadata:
        return DOWNLOADING_METADATA;
    case lt::torrent_status::downloading:
        return DOWNLOADING;
    case lt::torrent_status::finished:
        return FINISHED;
    case lt::torrent_status::seeding:
        return SEEDING;
    case lt::torrent_status::checking_resume_data:
        return CHECKING_RESUME_DATA;
    default:
        return UNDEFINED;
    }
}

torrent_status_t get_torrent_status(torrent_handle_t handle)
{
    std::optional<lt::torrent_status> opt_lt_status = TorrentPool::instance().get_torrent_status(handle->id);
    torrent_status_t status = (torrent_status_t)malloc(sizeof(_torrent_status_instance_t));
    if (opt_lt_status.has_value())
    {
        lt::torrent_status lt_status = opt_lt_status.value();
        status->type = state(lt_status.state);
        status->download_payload_rate = lt_status.download_payload_rate;
        status->total_done = lt_status.total_done;
        status->progress_ppm = lt_status.progress_ppm;
        status->num_peers = lt_status.num_peers;
    }
    else
    {
        status->type = UNDEFINED;
    }
    return status;
}

void create_magnet_uri(const char *file_name)
{
    lt::load_torrent_limits lt = {
        .max_buffer_size = 10000000,
        .max_pieces = 0x200000,
        .max_decode_depth = 100,
        .max_decode_tokens = 3000000};
    lt::torrent_info ti((std::string(file_name) + ".torrent"), lt);
    std::string uri(lt::make_magnet_uri(ti));

    MagnetPool::instance().add_magnet(file_name, uri);
}

const char *get_magnet_uri(const char *file_name)
{
    std::string uri = MagnetPool::instance().get_magnet(std::string(file_name));
    char *ret = (char *)malloc(uri.size() + 1);
    strcpy(ret, uri.c_str());
    return ret;
}

int set_dht_bootstrap_nodes(const char *node_list)
{
    std::string input(node_list);
    input.erase(std::remove_if(input.begin(), input.end(),
                               [](unsigned char c)
                               { return std::isspace(c); }),
                input.end());

    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, ','))
    {
        tokens.push_back(token);
    }

    std::vector<std::pair<std::string, int>> pairs;
    for (auto token : tokens)
    {
        std::stringstream sst(token);
        std::string url, port;
        std::getline(sst, url, ':');
        std::getline(sst, port);
        std::pair<std::string, int> pair = {url, std::stoi(port)};
        pairs.push_back(pair);
    }

    try
    {
        LibTorrentSession::instance().set_dht_bootstrap_nodes(pairs);
        std::cout << "Successfully set DHT bootstrap nodes:\n"
                  << node_list << std::endl;
    }
    catch (const std::exception &e)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

char *get_current_time()
{
    return (char *)(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}

char *post_to_dht(const char *file_name)
{
    try
    {
        LibTorrentSession::instance().create_torrent_file(file_name);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return NULL;
    }

    create_magnet_uri(file_name);

    std::string username = LibTorrentSession::instance().get_username();
    lt::entry metadata;

    lt::hasher hasher;

    const char *time = get_current_time();
    hasher.update(time, sizeof(time));
    metadata["id"] = hasher.final();
    metadata["magnet"] = get_magnet_uri(file_name);
    metadata["time"] = time;

    lt::entry data;
    hasher.update(username);
    data[hasher.final().data()] = metadata;

    std::vector<char> post_data;
    lt::bencode(std::back_inserter(post_data), data);

    lt::sha1_hash hash = LibTorrentSession::instance().post_to_dht(post_data);

    return hash.data();
}

dht_item_t *check_dht_queries(char *username)
{
    lt::hasher hasher;
    hasher.update(std::string(username));
    char *hashed_user = hasher.final().data();

    std::vector<lt::dht_immutable_item_alert *> alerts = LibTorrentSession::instance().get_dht_alerts();
    dht_item_t *ret_list = (dht_item_t *)malloc(alerts.size() * sizeof(dht_item_t));

    int i = 0;
    for (auto alert : alerts)
    {
        lt::entry *entry = alert->item.find_key(hashed_user);
        if (entry != nullptr)
        {
            strncpy(ret_list[i]->hash, alert->target.data(), sizeof(ret_list[i]->hash));
            strncpy(ret_list[i]->username, username, sizeof(ret_list[i]->username));
            strncpy(ret_list[i]->magnet, (*entry)["magnet"].string().c_str(), sizeof(ret_list[i]->magnet));
            strncpy(ret_list[i]->id, (*entry)["id"].string().c_str(), sizeof(ret_list[i]->id));
            ret_list[i]->time = (long)(*entry)["time"].string().c_str();
        }
        i++;
    }
    return ret_list;
}

void get_user_post_magnets(char *username)
{
    LibTorrentSession::instance().query_dht(username);
}

torrent_handle_t seed_file(post_bundle_t post)
{
    torrent_handle_t handle = (torrent_handle_t)malloc(sizeof(torrent_handle_t));
    try
    {
        lt::torrent_handle lt_handle = LibTorrentSession::instance().seed_torrent(post->path);
        MagnetPool::instance().add_magnet(std::string(post->title), std::string(post->magnet));
        TorrentPool::instance().add_torrent(lt_handle);
        std::cout << "Seeded file: " << post->title << std::endl;
        handle->id = lt_handle.id();
        strncpy(handle->name, lt_handle.status().name.c_str(), sizeof(handle->name));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return handle;
}