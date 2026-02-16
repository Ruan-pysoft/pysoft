//${

typedef struct NavLink {
    const char *name;
    const char *href;
    const char *id;
} NavLink;

static const NavLink nav_links[] = {
    { "Home",          "/",         "link-home-nav", },
    { "JS Test",       "/js",       NULL, },
    { "Blog",          "/blog",     NULL, },
    { "About",         "/about",    NULL, },
};
static const size_t nav_links_count = sizeof(nav_links)/sizeof(*nav_links);

//}$
