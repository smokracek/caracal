#include "../include/torrent_status.h"

typedef struct torrent_status_instance_t
{
    status_type_t type;
    int download_payload_rate;
    long total_done;
    int progress_ppm;
    int num_peers;
} *torrent_status_t;