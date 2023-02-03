#include "../include/torrent_handle.h"

typedef struct torrent_handle_instance_t
{
    unsigned int id;
    char name[128];
} *torrent_handle_t;