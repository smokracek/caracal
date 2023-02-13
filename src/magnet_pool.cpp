#include "magnet_pool.hpp"

MagnetPool &MagnetPool::instance()
{
    static MagnetPool *pool = new MagnetPool();
    return *pool;
}

MagnetPool::MagnetPool()
{
    std::unordered_map<std::string, std::string> pool_;
}

MagnetPool::~MagnetPool()
{
    pool_.~unordered_map();
}

void MagnetPool::add_magnet(std::string file_name, std::string magnet)
{
    pool_[file_name] = magnet;
}

std::string MagnetPool::get_magnet(std::string file_name)
{
    try
    {
        return pool_.at(file_name);
    }
    catch (std::exception &e)
    {
        return "";
    }
}

void MagnetPool::remove_magnet(std::string file_name)
{
    pool_.erase(file_name);
}

void MagnetPool::clear_magnets()
{
    pool_.clear();
}