#ifndef CARACAL_ALERTS_H
#define CARACAL_ALERTS_H

#ifdef __cplusplus
extern "C" {
#endif

enum AlertType {
    ADD_TORRENT,
    TORRENT_FINISHED,
    TORRENT_ERROR,
    SAVE_RESUME_DATA,
    SAVE_RESUME_DATA_FAILED,
    PIECE_FINISHED,
    TORRENT_STATE
};

typedef struct {

} add_torrent_alert;

typedef struct {
} torrent_finished_alert;

typedef struct {
} save_resume_data_alert;

typedef struct {
} save_resume_data_failed_alert;

typedef struct {
} piece_finished_alert;

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
        add_torrent_alert add_torrent;
        torrent_finished_alert torrent_finished;
        save_resume_data_alert save_resume_data;
        save_resume_data_failed_alert save_resume_data_failed;
        piece_finished_alert piece_finished;     
        torrent_state_alert torrent_state;   
    }; 
} alert;

#ifdef __cplusplus
}
#endif

#endif