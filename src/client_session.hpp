//
// Created by Sam Mokracek on 1/19/23.
//

#ifndef CARACAL_CLIENT_SESSION_HPP
#define CARACAL_CLIENT_SESSION_HPP

#include <libtorrent/session.hpp>

namespace floppa {

    class ClientSession {
    public:
        static ClientSession& instance();
        void addMagnet(const std::string& magnet_uri);
        void pauseSession();
        void resumeSession();
        void stopSession();
        void set_storage_dir(const std::string& path);
    private:
        ClientSession();
        ~ClientSession();

        lt::session session_;
        std::string storage_dir_;
    };

} // floppa

#endif //CARACAL_CLIENT_SESSION_HPP
