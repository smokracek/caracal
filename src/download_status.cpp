#include "download_status.h"

status_type_t get_status_type(download_status_t instance)
{
    return instance->type;
}

const char *get_status_type_string(download_status_t instance)
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

int get_download_payload_rate(download_status_t instance)
{
    return instance->download_payload_rate;
}

long get_total_done(download_status_t instance)
{
    return instance->total_done;
}

int get_progress_ppm(download_status_t instance)
{
    return instance->progress_ppm;
}

int get_num_peers(download_status_t instance)
{
    return instance->num_peers;
}