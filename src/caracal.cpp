#include "../include/libtorrent_session.hpp"
#include "../include/caracal.h"
#include "../include/alert.h"


void set_storage_dir(const char* path) {
    caracal::LibTorrentSession::instance().set_storage_dir(path);
}

void add_magnet(const char* magnet_uri) {
    return caracal::LibTorrentSession::instance().addMagnet(magnet_uri);
}

struct alert* get_alerts(int* size) {
    std::vector<lt::alert*> lt_alerts = caracal::LibTorrentSession::instance().get_session_alerts();
    *size = lt_alerts.size();
    struct alert* alerts = (struct alert*) malloc(*size * sizeof(alert));
    for (int i = 0; i < *size; i++) {
        strcpy(alerts[i].type, lt_alerts[i]->what());
        strcpy(alerts[i].message, lt_alerts[i]->message().c_str());
    }
    return alerts;
}

void free_alerts(struct alert* alerts) {
    free(alerts);
}

