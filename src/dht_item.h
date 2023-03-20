#include <time.h>
#include "../include/dht_item.h"

typedef struct _dht_item_instance_t
{
    long time;
    char magnet[256];
    char hash[20];
    char id[20];
    char username[256];
} *dht_item_t;