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
    {
        .id = "kernelcpp",
        .title = "C++ Demo Kernel",
        .description = "a bare-metal \"os\" with a few demo applications compiled in",
        .link = "https://github.com/Ruan-pysoft/cpp_demo_kernel",
    },
    {
        .id = "cruntime",
        .title = "C Runtime Library for Linux",
        .description = "a runtime library for C, no dependencies on libc; currently C89 but planning to move to C99",
        .link = "https://github.com/Ruan-pysoft/ministdlib",
    },
    {
        .id = "platformer",
        .title = "Platformer Game in C++ with Raylib",
        .description = "a small platformer game created for my CMPG121 project",
        .link = "https://github.com/Ruan-pysoft/platformer",
    },
    {
        .id = "imgavg",
        .title = "Image Averaging",
        .description = "a small C program to average multiple images together, emulating long exposure times",
        .link = "https://github.com/Ruan-pysoft/image_averaging",
    },
    {
        .id = "basic",
        .title = "BASIC Interpreter",
        .description = "interpreter for my own dialect of basic",
        .link = "https://git.sr.ht/~ruan_p/basic_interpreter",
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
