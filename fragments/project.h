//${

#include "common.h"

typedef struct Project {
    const char *id;
    const char *title;
    const char *description;
    const char *link;
} Project;

static const Project projects[] = {
    {
        .id = "rholisp",
        .title = "ρlisp",
        .description = "a small lisp implementation initially written on my phone",
        .link = "https://github.com/Ruan-pysoft/rholisp",
    },
};
static const size_t projects_count = sizeof(projects)/sizeof(*projects);

size_t project_get_item_ix(const char *id) {
    for (size_t i = 0; i < projects_count; ++i) {
        if (strcmp(projects[i].id, id) == 0) return i;
    }
    nob_log(ERROR, "Invalid project id: %s", id);
    exit(1);
    return 0;
}
Project project_get_item(const char *id) {
    return projects[project_get_item_ix(id)];
}

#define SIZE_T_MAX (~(size_t)0)
static size_t project_curr_idx = SIZE_T_MAX;
void project_begin(const char *id) {
    if (project_curr_idx != SIZE_T_MAX) {
        nob_log(
            ERROR, "Please end current project (id: %s) before starting a new one!",
            projects[project_curr_idx].id
        );
        nob_log(INFO, "You shouldn't have multiple projects inside one file anyways...");
        exit(1);
    }

    project_curr_idx = project_get_item_ix(id);

    Project project = projects[project_curr_idx];

    page_title = project.title;
    page_description = project.description;

    header();

    printf("<h1>%s</h1>", page_title);

    if (project.link) {
        printf("<p><sub>External project link: <a href=\"%s\">%s</a></sub></p>", project.link, project.link);
    }
}

void project_end() {
    printf("<p><a href=\"/project\">Back to list of projects</a></p>");

    footer();

    project_curr_idx = SIZE_T_MAX;
}

//}$
