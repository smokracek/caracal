#include "post_bundle.h"

const char *get_post_title(post_bundle_t instance)
{
    return instance->title;
}

const char *get_post_magnet(post_bundle_t instance)
{
    return instance->magnet;
}

const char *get_post_path(post_bundle_t instance)
{
    return instance->path;
}