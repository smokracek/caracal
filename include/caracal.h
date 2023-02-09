#ifndef CARACAL_CARACAL_H
#define CARACAL_CARACAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "torrent_status.h"
#include "torrent_handle.h"

    void set_storage_dir(const char *path);
    torrent_handle_t add_magnet(const char *magnet_uri);
    torrent_status_t get_torrent_status(torrent_handle_t);
    void handle_alerts();

#ifdef __cplusplus
}
#endif

#endif // CARACAL_CARACAL_H
