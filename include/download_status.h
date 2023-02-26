#ifndef CARACAL_DOWNLOAD_STATUS_H
#define CARACAL_DOWNLOAD_STATUS_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Holds snapshot status data for queried torrent.
     */
    typedef struct _download_status_instance_t *download_status_t;

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

    status_type_t get_status_type(download_status_t instance);
    const char *get_status_type_string(download_status_t instance);
    int get_download_payload_rate(download_status_t instance);
    long get_total_done(download_status_t instance);
    int get_progress_ppm(download_status_t instance);
    int get_num_peers(download_status_t instance);

#ifdef __cplusplus
}
#endif

#endif