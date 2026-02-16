${#include "nav.h"}$

${static const char *page_title = NULL;}$
${static const char *page_description = "Ruan's personal website - under construction...";}$
${static const char *head_extra = NULL;}$

${void header(void) {}$
${if (!page_title) {
    nob_log(INFO, "Set page title with $""{page_title = \"...\";}""$");
}}$
$[toggle_ignore]$
<!DOCTYPE html>
<html lang="en-ZA">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="$(page_description)$">
    <title>$(page_title ? "%s – " : "", page_title)$Pysoft</title>
    <link rel="stylesheet" href="/site.css">
    $(head_extra ? head_extra : "")$
</head>
<body>
    <header id="header">
        <nav id="navbar">
        ${for (size_t i = 0; i < nav_links_count; ++i) {}$
            ${const NavLink link = nav_links[i];}$
            <a href="$(link.href)$" ${if (link.id) {}$ id="$(link.id)$" ${}}$>${printf(link.name);}$</a>
        ${}}$
        </nav>
    </header>
    <main>
$[toggle_ignore]$
${}}$

${static const char *license_extra = NULL;}$

${void footer(void) {}$
$[toggle_ignore]$
    </main>
    <footer id="footer">
        <p>&copy; 2023&mdash;2025 <a href="https://pysoft.co.za">Ruan</a></p>
        <p>Some css from <a href="https://hastysite.h3rald.com">HastySite</a></p>
        <p>
            Website content licensed under the 
            <a href="https://creativecommons.org/licenses/by/4.0/">
                Creative Commons Attribution 4.0 International
            </a>
            license, unless otherwise specified.
        </p>
        $(license_extra ? license_extra : "")$
    </footer>
</body>
</html>
$[toggle_ignore]$
${}}$

${const char *asset(const char *name) {
    return temp_sprintf("/assets/%s", name);
}}$
${const char *image(const char *name) {
    return asset(temp_sprintf("/images/%s", name));
}}$

${
struct {
    const char **items;
    size_t count;
    size_t capacity;
} footnotes = {0};

void fn(void) {
    nob_da_append(&footnotes, NULL);

    const size_t fn_num = footnotes.count;
    printf("<sup><a href=\"#fn%lu\" id=\"fnref%lu\" class=\"footnote-ref\">%lu</a></sup>", fn_num, fn_num, fn_num);
}
void fn_text(const char *text) {
    size_t idx = 0;
    for (; idx < footnotes.count; ++idx) {
        if (footnotes.items[idx] == NULL) break;
    }
    if (idx == footnotes.count) {
        nob_log(ERROR, "Cannot create footnote text without footnote reference first!");
       return;
    }
    footnotes.items[idx] = text;
    const size_t fn_num = idx+1;
    printf("<div class=\"footnote\" id=\"fn%lu\"><sup><a href=\"#fnref%lu\" class=\"footnote-backref\">^%lu</a></sup> %s</div>", fn_num, fn_num, fn_num, text);
}
}$
