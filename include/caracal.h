#ifndef CARACAL_CARACAL_H
#define CARACAL_CARACAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char state[32];
    int download_rate;
    long total_done;
    int progress_ppm;
    int num_peers;
} torrent_stats;

typedef struct {
    char type[32];
    char message[256];
    char has_stats;
    torrent_stats stats;
} alert;

void set_storage_dir(const char* path);
void add_magnet(const char* magnet_uri);
alert* get_alerts(int* size);
void free_alerts(alert* alerts);

#ifdef __cplusplus
}
#endif

#endif //CARACAL_CARACAL_H
