#include "torrent_handle.h"

unsigned int get_torrent_id(torrent_handle_t instance)
{
    return instance->id;
}

char *get_torrent_name(torrent_handle_t instance)
{
    return instance->name;
}
