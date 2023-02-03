#include "../include/caracal.h"
#include "libtorrent_session.hpp"
#include "torrent_pool.hpp"
#include "torrent_status.h"
#include "torrent_handle.h"
#include <libtorrent/torrent_status.hpp>

void set_storage_dir(const char *path)
{
    caracal::LibTorrentSession::instance().set_storage_dir(path);
}

void add_magnet(const char *magnet_uri)
{
    return caracal::LibTorrentSession::instance().add_magnet(magnet_uri);
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
    std::optional<lt::torrent_status> opt_lt_status =
        caracal::TorrentPool::instance().get_torrent_status(handle->id);
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
