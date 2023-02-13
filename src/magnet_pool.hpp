#ifndef CARACAL_MAGNET_POOL_HTTP
#define CARACAL_MAGNET_POOL_HTTP

#include <unordered_map>
#include <string>

class MagnetPool
{
public:
    static MagnetPool &instance();
    void add_magnet(std::string file_name, std::string magnet);
    std::string get_magnet(std::string file_name);
    void remove_magnet(std::string file_name);
    void clear_magnets();

private:
    MagnetPool();
    ~MagnetPool();

    std::unordered_map<std::string, std::string> pool_;
};

#endif