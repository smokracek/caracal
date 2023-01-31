#include "../include/libtorrent_session.hpp"
#include "../include/caracal.h"
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert_types.hpp>


void set_storage_dir(const char* path) {
    caracal::LibTorrentSession::instance().set_storage_dir(path);
}

void add_magnet(const char* magnet_uri) {
    return caracal::LibTorrentSession::instance().add_magnet(magnet_uri);
}

char const* state(lt::torrent_status::state_t s) {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#endif
    switch(s) {
        case lt::torrent_status::checking_files: return "checking";
        case lt::torrent_status::downloading_metadata: return "dl metadata";
        case lt::torrent_status::downloading: return "downloading";
        case lt::torrent_status::finished: return "finished";
        case lt::torrent_status::seeding: return "seeding";
        case lt::torrent_status::checking_resume_data: return "checking resume";
        default: return "<>";
    }
#ifdef __clang_
#pragma clang diagnostic pop
#endif
}

alert* get_alerts(int* size) {
    std::vector<lt::alert*> lt_alerts = caracal::LibTorrentSession::instance().get_session_alerts();
    *size = lt_alerts.size();
    alert* alerts = (alert*) malloc(*size * sizeof(alert));
    for (int i = 0; i < *size; i++) {
        strncpy(alerts[i].type, lt_alerts[i]->what(), sizeof(alerts[i].type));
        strncpy(alerts[i].message, lt_alerts[i]->message().c_str(), sizeof(alerts[i].message));

        if (auto st = lt::alert_cast<lt::state_update_alert>(lt_alerts[i])) {
            if (st->status.empty()) continue;

            lt::torrent_status const& s = st->status[0];
            torrent_stats stats;
            strcpy(stats.state, state(s.state));
            stats.download_rate = s.download_payload_rate;
            stats.total_done = (long) s.total_done;
            stats.progress_ppm = s.progress_ppm;
            stats.num_peers = s.num_peers;

            alerts[i].has_stats = 1;
            alerts[i].stats = stats;
        } else {
            alerts[i].has_stats = 0;
        }

        if (auto at = lt::alert_cast<lt::torrent_added_alert>(lt_alerts[i])) {
            caracal::LibTorrentSession::instance().add_handle(at->torrent_name(), at->handle);
        }

        if (auto af = lt::alert_cast<lt::torrent_finished_alert>(lt_alerts[i])) {
            caracal::LibTorrentSession::instance()
                    .get_handle(af->torrent_name())
                    .save_resume_data(lt::torrent_handle::only_if_modified
                                   | lt::torrent_handle::save_info_dict);
        }
    }
    return alerts;
}

void free_alerts(alert* alerts) {
    free(alerts);
}

