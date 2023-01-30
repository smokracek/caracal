#ifndef CARACAL_LIBTORRENT_SESSION_HPP
#define CARACAL_LIBTORRENT_SESSION_HPP

#include <libtorrent/session.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/alert.hpp>
#include <string>
#include <vector>
#include "alert.h"

namespace caracal {

    class LibTorrentSession {
    public:
        static LibTorrentSession& instance();
        void addMagnet(const std::string& magnet_uri);
        void set_storage_dir(const std::string& path);
        std::vector<lt::alert*> get_session_alerts();
    private:
        LibTorrentSession();
        ~LibTorrentSession();

        lt::session session_;
        std::string storage_dir_;
    };

} // floppa

#endif //CARACAL_CLIENT_SESSION_HPP
