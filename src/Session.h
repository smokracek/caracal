//
// Created by Sam Mokracek on 1/19/23.
//

#ifndef CARACAL_SESSION_H
#define CARACAL_SESSION_H

#include <libtorrent/session.hpp>

namespace caracal {

    class Session {
    public:
        static Session& instance();
        void startSession();
        void addMagnet(const std::string& magnetUri);
        void pauseSession();
        void resumeSession();
        void stopSession();
    private:
        Session();
        ~Session();

        lt::session session_;
    };

} // caracal

#endif //CARACAL_SESSION_H
