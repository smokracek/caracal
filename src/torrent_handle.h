#include "../include/torrent_handle.h"

typedef struct _torrent_handle_instance_t
{
    unsigned int id;
    char name[128];
} *torrent_handle_t;