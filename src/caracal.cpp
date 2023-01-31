#include "../include/libtorrent_session.hpp"
#include "../include/caracal.h"
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/write_resume_data.hpp>
#include <fstream>
#include <iostream>


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
        lt::alert* a = lt_alerts[i];
        lt::torrent_handle h;
        if (auto at = lt::alert_cast<lt::add_torrent_alert>(a)) {
            h = at->handle;
        }
        // if we receive the finished alert or an error, we're done
        if (lt::alert_cast<lt::torrent_finished_alert>(a)) {
            h.save_resume_data(lt::torrent_handle::only_if_modified
                                | lt::torrent_handle::save_info_dict);
        }
        if (lt::alert_cast<lt::torrent_error_alert>(a)) {
            std::cout << a->message() << std::endl;
            h.save_resume_data(lt::torrent_handle::only_if_modified
                                | lt::torrent_handle::save_info_dict);
        }

        // when resume data is ready, save it
        if (auto rd = lt::alert_cast<lt::save_resume_data_alert>(a)) {
            std::ofstream of(".resume_file", std::ios_base::binary);
            of.unsetf(std::ios_base::skipws);
            auto const b = write_resume_data_buf(rd->params);
            of.write(b.data(), int(b.size()));
        }

        if (lt::alert_cast<lt::save_resume_data_failed_alert>(a)) {
        }

        if (auto st = lt::alert_cast<lt::state_update_alert>(a)) {
            if (st->status.empty()) continue;

            // we only have a single torrent, so we know which one
            // the status is for
            lt::torrent_status const& s = st->status[0];
            std::cout << '\r' << state(s.state) << ' '
                        << (s.download_payload_rate / 1000) << " kB/s "
                        << (s.total_done / 1000) << " kB ("
                        << (s.progress_ppm / 10000) << "%) downloaded ("
                        << s.num_peers << " peers)\x1b[K";
            std::cout.flush();
        }
    }
    return alerts;
}

void free_alerts(alert* alerts) {
    free(alerts);
}

