#ifndef CARACAL_POST_BUNDLE_H
#define CARACAL_POST_BUNDLE_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct post_bundle_instance_t *post_bundle_t;

    const char *get_post_title(post_bundle_t instance);
    const char *get_post_magnet(post_bundle_t instance);

#ifdef __cplusplus
}
#endif

#endif