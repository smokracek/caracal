#ifndef CARACAL_POST_BUNDLE_H
#define CARACAL_POST_BUNDLE_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Holds info about a post.
     */
    typedef struct _post_bundle_instance_t *post_bundle_t;

    /**
     * Gets the title of the post. Derived from original post file name.
     * @param instance A post bundle.
     * @return The current title of the post.
     */
    const char *get_post_title(post_bundle_t instance);

    /**
     * Gets the magnet link for the post.
     * @param instance A post bundle.
     * @return The generated magnet link for the post.
     */
    const char *get_post_magnet(post_bundle_t instance);

#ifdef __cplusplus
}
#endif

#endif