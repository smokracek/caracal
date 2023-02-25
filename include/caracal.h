#ifndef CARACAL_CARACAL_H
#define CARACAL_CARACAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "torrent_status.h"
#include "torrent_handle.h"
#include "post_bundle.h"

    /**
     * Sets the storage directory for downloaded files.
     * @param path absolute path to directory.
     */
    void set_storage_dir(const char *path);

    /**
     * Initiates a torrent using the provided magnet link.
     * @param magnet_uri A valid magnet link.
     * @return A handle to the torrent.
     */
    torrent_handle_t add_magnet(const char *magnet_uri);

    /**
     * Returns a status about the handle's corresponding torrent.
     * @param handle Handle to an initiated torrent.
     * @return An object holding a snapshot of download state.
     */
    torrent_status_t get_torrent_status(torrent_handle_t handle);

    /**
     * To be deprecated. Opaquely handles the libTorrent session alerts.
     */
    void handle_alerts(void);

    /**
     * Creates a torrent file and post bundle.
     * @param file_name The relative path to the file.
     * @return A post bundle descriptor of the torrent.
     */
    post_bundle_t create_post(const char *file_name);

    /**
     * Sets servers to use to bootstrap the DHT network connection.
     * @param ip_port_list A comma separated list of nodes in `0.0.0.0:0000` format.
     */
    int set_dht_bootstrap_nodes(const char *ip_port_list);

#ifdef __cplusplus
}
#endif

#endif // CARACAL_CARACAL_H
