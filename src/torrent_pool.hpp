#ifndef CARACAL_TORRENT_POOL_HPP
#define CARACAL_TORRENT_POOL_HPP

#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/torrent_status.hpp>
#include <unordered_map>
#include <optional>

namespace caracal
{

    class TorrentPool
    {
    public:
        static TorrentPool &instance();
        void add_torrent(lt::torrent_handle handle);
        lt::torrent_handle get_torrent(unsigned int torrent_id);
        void remove_torrent(unsigned int torrent_id);
        void clear_torrents();
        std::optional<lt::torrent_status> get_torrent_status(unsigned int id);

    private:
        TorrentPool();
        ~TorrentPool();

        std::unordered_map<unsigned int, lt::torrent_handle> pool_;
    };

} // caracal

#endif