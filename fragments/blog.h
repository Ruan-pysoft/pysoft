//${

#include "common.h"

typedef struct BlogEntry {
    const char *id;
    const char *title;
    const char *description;
    const char *pub_date;
} BlogEntry;

static const BlogEntry blog_entries[] = {
    {
        .id = "magickgreet",
        .title = "“Simple” Bash Hello World",
        .description = "a 'simple' hello world program in bash",
        .pub_date = "2025-10-25",
    },
    {
        .id = "website",
        .title = "Finally, a website!",
        .description = "(TODO)",
        .pub_date = "2026-02-16",
    },
};
static const size_t blog_entries_count = sizeof(blog_entries)/sizeof(*blog_entries);

size_t blog_get_entry_ix(const char *id) {
    for (size_t i = 0; i < blog_entries_count; ++i) {
        if (strcmp(blog_entries[i].id, id) == 0) return i;
    }
    nob_log(ERROR, "Invalid blog entry id: %s", id);
    exit(1);
    return 0;
}
BlogEntry blog_get_entry(const char *id) {
    return blog_entries[blog_get_entry_ix(id)];
}

#define SIZE_T_MAX (~(size_t)0)
static size_t blog_curr_article_idx = SIZE_T_MAX;
void blog_article_begin(const char *id) {
    if (blog_curr_article_idx != SIZE_T_MAX) {
        nob_log(
            ERROR, "Please end current article (id: %s) before starting a new one!",
            blog_entries[blog_curr_article_idx].id
        );
        nob_log(INFO, "You shouldn't have multiple entries inside one file anyways...");
        exit(1);
    }

    blog_curr_article_idx = blog_get_entry_ix(id);

    BlogEntry entry = blog_entries[blog_curr_article_idx];

    page_title = entry.title;
    page_description = entry.description;

    header();

    printf("<h1>%s</h1>", page_title);
}

void blog_article_end() {
    const char *prev_link = blog_curr_article_idx > 0
        ? temp_sprintf("<a href=\"/blog/%s\" class=\"prev-link\">← Previous entry | %s</a>", blog_entries[blog_curr_article_idx-1].id, blog_entries[blog_curr_article_idx-1].title)
        : NULL;
    const char *next_link = blog_curr_article_idx < ARRAY_LEN(blog_entries)-1
        ? temp_sprintf("<a href=\"/blog/%s\" class=\"next-link\">%s | Next entry →</a>", blog_entries[blog_curr_article_idx+1].id, blog_entries[blog_curr_article_idx+1].title)
        : NULL;

    if (prev_link && next_link) {
        printf("<div class=\"series-link-container\">%s — %s</div>", prev_link, next_link);
    } else if (prev_link) {
        printf("<div class=\"series-link-container\">%s</div>", prev_link);
    } else if (next_link) {
        printf("<div class=\"series-link-container\">%s</div>", next_link);
    }

    footer();

    blog_curr_article_idx = SIZE_T_MAX;
}

//}$
