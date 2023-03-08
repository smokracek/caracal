#include "torrent_status.h"

status_type_t get_status_type(torrent_status_t instance)
{
    return instance->type;
}

const char *get_status_type_string(torrent_status_t instance)
{
    const char *status_type_t_strings[STATUS_TYPE_T_COUNT] = {
        "CHECKING_FILES",
        "DOWNLOADING_METADATA",
        "DOWNLOADING",
        "FINISHED",
        "SEEDING",
        "CHECKING_RESUME_DATA",
        "UNDEFINED"};
    return status_type_t_strings[instance->type];
}

int get_torrent_payload_rate(torrent_status_t instance)
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