#ifndef CARACAL_DHT_ITEM_H
#define CARACAL_DHT_ITEM_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Holds info about a DHT post item
     */
    typedef struct _dht_item_instance_t *dht_item_t;

    /**
     * Returns the Unix time that the DHT item was posted
     * @return A long representing the Unix time when the item was posted
     */
    long get_dht_item_post_time(dht_item_t instance);

    /**
     * The magnet link associated with the post
     * @return the magnet link for the post torrent
     */
    char *get_dht_item_magnet(dht_item_t instance);

    /**
     * The username of the poster
     * @return the username of the poster
     */
    char *get_dht_item_username(dht_item_t instance);

    /**
     * The SHA-1 hash of the item
     * @return the hash
     */
    char *get_dht_item_hash(dht_item_t instance);

    /**
     * Gets the ID (hashed date of post)
     * @return The ID
     */
    char *get_dht_item_id(dht_item_t instance);

#ifdef __cplusplus
}
#endif

#endif