#include "../include/post_bundle.h"

typedef struct _post_bundle_instance_t
{
    char title[256];
    char magnet[1024];
    char path[256];
} *post_bundle_t;