//
// Created by Sam Mokracek on 1/19/23.
//

#include "Session.h"
#include <libtorrent/magnet_uri.hpp>

namespace caracal {

    Session& Session::instance() {
        static Session session;
        return session;
    }

    Session::Session() {
        // TODO initialize things?
    }

    Session::~Session() {
        session_.~session();
    }

    void Session::addMagnet(const std::string& magnetUri) {
        lt::add_torrent_params const params = lt::parse_magnet_uri(magnetUri);
        session_.async_add_torrent(params);
    }

    void Session::pauseSession() {
        session_.pause();
    }

    void Session::resumeSession() {
        session_.resume();
    }

    void Session::stopSession() {
        session_.abort();
    };
} // caracal