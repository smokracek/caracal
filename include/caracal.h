#ifndef CARACAL_CARACAL_H
#define CARACAL_CARACAL_H

#include "alerts.h"

#ifdef __cplusplus
extern "C" {
#endif

void set_storage_dir(const char* path);
void add_magnet(const char* magnet_uri);
alert* get_alerts(int* size);
void free_alerts(alert* alerts);

#ifdef __cplusplus
}
#endif

#endif //CARACAL_CARACAL_H
