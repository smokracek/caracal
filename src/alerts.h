#include "../include/alerts.h"
#include "../include/torrent_status.h"

typedef enum
{
    STATUS,
    DHT
} _alert_category_t;

typedef struct _status_alert_instance_t
{
    torrent_status_t status;
} *status_alert_t;

typedef struct _dht_alert_instance_t
{

} *dht_alert_t;

typedef struct _session_alert_instance_t
{
    _alert_category_t category;
    union alert
    {
        status_alert_t status;
        dht_alert_t dht;
    };

} session_alert_t;