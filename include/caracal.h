#ifndef CARACAL_CARACAL_H
#define CARACAL_CARACAL_H

#ifdef __cplusplus
extern "C" {
#endif

enum AlertType {
    TORRENT_STATE
};

typedef struct {
    char state[256];
    int download_payload_rate;
    long total_done;
    int progress_ppm;
    int num_peers;
} torrent_state_alert;

typedef struct {
    AlertType type;
    union {
        torrent_state_alert torrent_state;        
    }; 
} alert;

void set_storage_dir(const char* path);
void add_magnet(const char* magnet_uri);
alert* get_alerts(int* size);
void free_alerts(alert* alerts);

#ifdef __cplusplus
}
#endif

#endif //CARACAL_CARACAL_H
