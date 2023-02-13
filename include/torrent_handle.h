#ifndef CARACAL_TORRENT_HANDLE_H
#define CARACAL_TORRENT_HANDLE_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct torrent_handle_instance_t *torrent_handle_t;

    unsigned int get_torrent_id(torrent_handle_t instance);
    char *get_torrent_name(torrent_handle_t instance);

#ifdef __cplusplus
}
#endif

#endif