#include "torrent_pool.hpp"
#include <stdexcept>

TorrentPool &TorrentPool::instance()
{
    static TorrentPool *pool = new TorrentPool();
    return *pool;
}

TorrentPool::TorrentPool()
{
    std::unordered_map<unsigned int, lt::torrent_handle> pool_;
}

TorrentPool::~TorrentPool()
{
    pool_.~unordered_map();
}

void TorrentPool::add_torrent(lt::torrent_handle handle)
{
    pool_[handle.id()] = handle;
}

lt::torrent_handle TorrentPool::get_torrent(unsigned int id)
{
    try
    {
        pool_.at(id);
    }
    catch (std::exception &e)
    {
        throw std::invalid_argument(
            "Torrent handle not found with matching id " + std::to_string(id));
    }
}

void TorrentPool::remove_torrent(unsigned int id)
{
    pool_.erase(id);
}

void TorrentPool::clear_torrents()
{
    pool_.clear();
}

std::optional<lt::torrent_status> TorrentPool::get_torrent_status(unsigned int id)
{
    try
    {
        lt::torrent_handle handle = pool_.at(id);
        return std::optional<lt::torrent_status>{handle.status()};
    }
    catch (std::exception &e)
    {
        return std::nullopt;
    }
}
