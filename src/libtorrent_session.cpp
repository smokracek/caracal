#include "libtorrent_session.hpp"
#include "torrent_pool.hpp"
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/session_params.hpp>
#include <libtorrent/settings_pack.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/file_storage.hpp>
#include <libtorrent/create_torrent.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/entry.hpp>
#include <fstream>
#include <iostream>
#include <functional>

LibTorrentSession &LibTorrentSession::instance()
{
    static LibTorrentSession *session = new LibTorrentSession();
    return *session;
}

LibTorrentSession::LibTorrentSession()
{
    set_download_storage_dir(".");
    lt::session_params params;
    params.settings.set_int(
        lt::settings_pack::alert_mask,
        lt::alert_category::status | lt::alert_category::error | lt::alert_category::storage | lt::alert_category::piece_progress);
    lt::session session_(params);
};

LibTorrentSession::~LibTorrentSession()
{
    session_.~session();
}

lt::torrent_handle LibTorrentSession::add_magnet(const std::string &magnet_uri)
{
    std::ifstream ifs(".resume_file", std::ios_base::binary);
    ifs.unsetf(std::ios_base::skipws);
    std::vector<char> buf{
        std::istream_iterator<char>(ifs),
        std::istream_iterator<char>()};

    lt::add_torrent_params magnet = lt::parse_magnet_uri(magnet_uri);
    if (!buf.empty())
    {
        lt::add_torrent_params atp = lt::read_resume_data(buf);
        if (atp.info_hashes == magnet.info_hashes)
        {
            magnet = std::move(atp);
        }
    }

    magnet.save_path = download_storage_dir_;
    lt::torrent_handle handle = session_.add_torrent(std::move(magnet));
    TorrentPool::instance().add_torrent(handle);
    return handle;
}

void LibTorrentSession::set_download_storage_dir(const std::string &path)
{
    LibTorrentSession::download_storage_dir_ = path;
}

void LibTorrentSession::set_post_storage_dir(const std::string &path)
{
    LibTorrentSession::post_storage_dir_ = path;
}

std::vector<lt::alert *> LibTorrentSession::get_session_alerts()
{
    session_.post_torrent_updates();
    std::vector<lt::alert *> lt_alerts;
    session_.pop_alerts(&lt_alerts);
    return lt_alerts;
}

void LibTorrentSession::create_torrent_file(const std::string &file_path)
{

    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    std::int64_t size = file.tellg();
    file.close();

    std::string outfile = file_path + ".torrent";
    std::string parent_dir = file_path.substr(0, file_path.find_last_of("/\\"));

    lt::file_storage fs;
    int piece_size = 0;
    lt::create_flags_t flags = {};
    fs.add_file(file_path, size);

    lt::create_torrent t(fs, piece_size, flags);

    lt::error_code ec;
    lt::set_piece_hashes(t, parent_dir, ec);

    if (ec)
    {
        std::cout << "Error setting piece hashes: " << ec.message() << std::endl;
        return;
    }

    std::vector<char> torrent;
    lt::bencode(back_inserter(torrent), t.generate());
    std::fstream out;
    out.open(outfile.c_str(), std::ios_base::out | std::ios_base::binary);
    out.write(torrent.data(), int(torrent.size()));
}
