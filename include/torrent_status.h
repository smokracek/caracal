#ifndef CARACAL_TORRENT_STATUS_H
#define CARACAL_TORRENT_STATUS_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Holds snapshot status data for queried torrent.
     */
    typedef struct torrent_status_instance_t *torrent_status_t;

    /**
     * Possible status types.
     * Also includes the count of possible status types, always as the last member,
     * for easy creation of indexed string translation arrays.
     */
    typedef enum
    {
        CHECKING_FILES,
        DOWNLOADING_METADATA,
        DOWNLOADING,
        FINISHED,
        SEEDING,
        CHECKING_RESUME_DATA,
        UNDEFINED,
        STATUS_TYPE_T_COUNT
    } status_type_t;

    status_type_t get_status_type(torrent_status_t instance);
    const char *get_status_type_string(torrent_status_t instance);
    int get_download_payload_rate(torrent_status_t instance);
    long get_total_done(torrent_status_t instance);
    int get_progress_ppm(torrent_status_t instance);
    int get_num_peers(torrent_status_t instance);

#ifdef __cplusplus
}
#endif

#endif