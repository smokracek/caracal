#include "../include/download_status.h"

typedef struct _download_status_instance_t
{
    status_type_t type;
    int download_payload_rate;
    long total_done;
    int progress_ppm;
    int num_peers;
} *download_status_t;