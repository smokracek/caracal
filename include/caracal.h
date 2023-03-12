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
     * Initializes the session.
     */
    void init_session(void);

    /**
     * Sets the storage directory for downloaded files.
     * @param path absolute path to directory.
     */
    void set_post_storage_dir(const char *path);

    /**
     * Initiates a post download using the provided magnet link.
     * @param magnet_uri A valid magnet link.
     * @return A handle to the torrent.
     */
    torrent_handle_t download_post(const char *magnet_uri);

    /**
     * Returns a status about the handle's corresponding torrent.
     * @param handle Handle to an initiated torrent.
     * @return An object holding a snapshot of torrent state.
     */
    torrent_status_t get_torrent_status(torrent_handle_t handle);

    /**
     * To be deprecated. Opaquely handles the libTorrent session alerts.
     */
    void handle_alerts(void);

    /**
     * Creates a post bundle.
     * @param file_name The relative path to the file.
     * @return A post bundle descriptor of the torrent.
     */
    post_bundle_t create_post(const char *file_name);

    /**
     * Sets servers to use to bootstrap the DHT network connection.
     * @param url_port_list A comma separated list of nodes in `url:xxxx` format.
     * @return Integer representing success, 0 - success, 1 - failure
     */
    int set_dht_bootstrap_nodes(const char *url_port_list);

    /**
     * Sets the URL of the magnet repository server
     * @param url_port_pair A string in the format `url:xxxx`.
     * @return Integer representing success, 0 - success, 1 - failure
     */
    int set_magnet_repo(const char *url_port_pair);

    /**
     * Sends the post to the set magnet repo
     * @param post Post bundle containing info about the post
     * @return Integer representing success, 0 - success, 1 - failure
     */
    int send_post(post_bundle_t post);

    /**
     * Begins seeding the file the post bundle points to.
     * @param post Post bundle containing info about the post
     * @return Integer representing success, 0 - success, 1 - failure
     */
    torrent_handle_t seed_file(post_bundle_t post);

#ifdef __cplusplus
}
#endif

#endif // CARACAL_CARACAL_Hn