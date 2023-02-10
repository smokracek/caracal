#include "../include/caracal.h"
#include "libtorrent_session.hpp"
#include "torrent_pool.hpp"
#include "torrent_status.h"
#include "torrent_handle.h"
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/write_resume_data.hpp>
#include <fstream>
#include <iostream>

void set_storage_dir(const char *path)
{
    LibTorrentSession::instance().set_storage_dir(path);
}

torrent_handle_t add_magnet(const char *magnet_uri)
{
    lt::torrent_handle lt_handle = LibTorrentSession::instance().add_magnet(magnet_uri);
    torrent_handle_t handle = (torrent_handle_t)malloc(sizeof(torrent_handle_instance_t));
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
    torrent_status_t status = (torrent_status_t)malloc(sizeof(torrent_status_instance_t));
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
