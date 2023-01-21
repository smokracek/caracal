//
// Created by Sam Mokracek on 1/19/23.
//

#ifndef CARACAL_FLOPPA_SESSION_HPP
#define CARACAL_FLOPPA_SESSION_HPP

#include <libtorrent/session.hpp>

namespace floppa {

    class Session {
    public:
        static Session& instance();
        void addMagnet(const std::string& magnet_uri);
        void pauseSession();
        void resumeSession();
        void stopSession();
        void set_storage_dir(const std::string& path);
    private:
        Session();
        ~Session();

        lt::session session_;
        std::string storage_dir_;
    };

} // floppa

#endif //CARACAL_FLOPPA_SESSION_HPP
