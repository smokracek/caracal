#ifndef CARACAL_CARACAL_H
#define CARACAL_CARACAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "torrent_status.h"
#include "torrent_handle.h"

    void set_storage_dir(const char *path);
    void add_magnet(const char *magnet_uri);
    torrent_status_t get_torrent_status(torrent_handle_t);

#ifdef __cplusplus
}
#endif

#endif // CARACAL_CARACAL_H
