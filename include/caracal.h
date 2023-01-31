#ifndef CARACAL_CARACAL_H
#define CARACAL_CARACAL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char what[32];
    char message[256];
} alert;

void set_storage_dir(const char* path);
void add_magnet(const char* magnet_uri);
alert* get_alerts(int* size);
void free_alerts(alert* alerts);

#ifdef __cplusplus
}
#endif

#endif //CARACAL_CARACAL_H
