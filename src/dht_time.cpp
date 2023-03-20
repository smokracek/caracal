#include "dht_item.h"

long get_dht_item_post_time(dht_item_t instance)
{
    return (long)instance->time;
}

char *get_dht_item_magnet(dht_item_t instance)
{
    return instance->magnet;
}

char *get_dht_item_username(dht_item_t instance)
{
    return instance->username;
}

char *get_dht_item_hash(dht_item_t instance)
{
    return instance->hash;
}

char *get_dht_item_id(dht_item_t instance)
{
    return instance->id;
}