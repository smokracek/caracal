#include "torrent_status.h"
#include <cstdlib>

status_type_t get_status_type(torrent_status_t instance)
{
    return instance->type;
}

int get_download_payload_rate(torrent_status_t instance)
{
    return instance->download_payload_rate;
}

long get_total_done(torrent_status_t instance)
{
    return instance->total_done;
}

int get_progress_ppm(torrent_status_t instance)
{
    return instance->progress_ppm;
}

int get_num_peers(torrent_status_t instance)
{
    return instance->num_peers;
}

void free_torrent_statuses(torrent_status_t *statuses)
{
    free(statuses);
}