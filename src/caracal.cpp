#include "../include/caracal.h"
#include "libtorrent_session.hpp"
#include "torrent_pool.hpp"
#include "torrent_status.h"
#include "torrent_handle.h"
#include "post_bundle.h"
#include "magnet_pool.hpp"
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/write_resume_data.hpp>
#include <libtorrent/magnet_uri.hpp>
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

void handle_alerts()
{
    std::vector<lt::alert *> lt_alerts = LibTorrentSession::instance().get_session_alerts();
    for (int i = 0; i < lt_alerts.size(); i++)
    {
        lt::alert *a = lt_alerts[i];
        lt::torrent_handle h;
        if (auto at = lt::alert_cast<lt::add_torrent_alert>(a))
        {
            h = at->handle;
        }
        // if we receive the finished alert or an error, we're done
        if (lt::alert_cast<lt::torrent_finished_alert>(a))
        {
            h.save_resume_data(lt::torrent_handle::only_if_modified | lt::torrent_handle::save_info_dict);
        }
        if (lt::alert_cast<lt::torrent_error_alert>(a))
        {
            std::cout << a->message() << std::endl;
            h.save_resume_data(lt::torrent_handle::only_if_modified | lt::torrent_handle::save_info_dict);

            // when resume data is ready, save it
            if (auto rd = lt::alert_cast<lt::save_resume_data_alert>(a))
            {
                std::ofstream of(".resume_file", std::ios_base::binary);
                of.unsetf(std::ios_base::skipws);
                auto const b = write_resume_data_buf(rd->params);
                of.write(b.data(), int(b.size()));
            }
        }
    }
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

post_bundle_t create_post(const char *file_name)
{
    post_bundle_t post = (post_bundle_t)malloc(sizeof(_post_bundle_instance_t));
    strncpy(post->path, file_name, sizeof(post->path));

    try
    {
        LibTorrentSession::instance().create_torrent_file(std::string(file_name));
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << "\n"
                  << "Returning to main." << std::endl;
        return post;
    }

    create_magnet_uri(file_name);
    strncpy(post->title, file_name, sizeof(post->title));
    strncpy(post->magnet, get_magnet_uri(file_name), sizeof(post->magnet));

    return post;
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