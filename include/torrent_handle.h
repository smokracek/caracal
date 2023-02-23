#ifndef CARACAL_TORRENT_HANDLE_H
#define CARACAL_TORRENT_HANDLE_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Holds info about an initialized torrent.
     */
    typedef struct _torrent_handle_instance_t *torrent_handle_t;

    /**
     * Gives the unique (to this session) ID of an initialized torrent.
     * @param instance The handle of an initialized torrent.
     * @return The unique (to this session) ID of the initialized torrent.
     */
    unsigned int get_torrent_id(torrent_handle_t instance);

    /**
     * The name of an initialized torrent.
     * @param The handle of an initialized torrent.
     * @return The name of the initialized torrent.
     */
    char *get_torrent_name(torrent_handle_t instance);

#ifdef __cplusplus
}
#endif

#endif