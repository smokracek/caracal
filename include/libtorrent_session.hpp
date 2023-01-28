#ifndef CARACAL_LIBTORRENT_SESSION_HPP
#define CARACAL_LIBTORRENT_SESSION_HPP

#include <libtorrent/session.hpp>

namespace floppa {

    class LibTorrentSession {
    public:
        static LibTorrentSession& instance();
        void addMagnet(const std::string& magnet_uri);
        void pauseSession();
        void resumeSession();
        void stopSession();
        void set_storage_dir(const std::string& path);
        std::vector<std::string> get_alerts();
    private:
        LibTorrentSession();
        ~LibTorrentSession();

        lt::session session_;
        std::string storage_dir_;
    };

} // floppa

#endif //CARACAL_CLIENT_SESSION_HPP
