${
struct blip {
    unsigned year;
    unsigned month;
    unsigned day;
    unsigned index;

    const char *slug;
    bool has_time;
    unsigned hour;
    unsigned minute;

    const char *content;
};

struct blip parse_blip(const char *blip_name, const char *blip_path) {
    assert(strlen(blip_name) == 11 && "blip file name must be in the format YYYYMMDDIII");

    for (size_t i = 0; i < 11; ++i) {
        assert('0' <= blip_name[i] && blip_name[i] <= '9');
    }

    const unsigned year = (blip_name[0]-'0')*1000 + (blip_name[1]-'0')*100 + (blip_name[2]-'0')*10 + (blip_name[3]-'0');
    const unsigned month = (blip_name[4]-'0')*10 + (blip_name[5]-'0');
    const unsigned day = (blip_name[6]-'0')*10 + (blip_name[7]-'0');
    const unsigned index = (blip_name[8]-'0')*100 + (blip_name[9]-'0')*10 + (blip_name[10]-'0');

    assert(0 < month && month <= 12 && "month must be in the range 1-12");
    assert(0 < day && day <= 31 && "day must be in the range 1-31");

    String_Builder content = {0};
    if (!nob_read_entire_file(blip_path, &content)) exit(1);

    struct blip blip = {
        .year = year,
        .month = month,
        .day = day,
        .index = index,

        .slug = NULL,
        .has_time = false,
        .hour = 0,
        .minute = 0,

        .content = NULL,
    };

    // structure:
    // <slug> [<time>]
    // <blank line>
    // <content>
    // for now assume the content can fit in a single <p> tag

#define adv() do { ++content.items; --content.count; } while (0)
#define skip_space() while (content.count && *content.items <= ' ' && *content.items != '\n') adv()

    skip_space();
    const char *slug_begin = content.items;
    while (content.count && *content.items > ' ') adv();
    const size_t slug_len = content.items - slug_begin;
    assert(slug_len > 0);
    blip.slug = strndup(slug_begin, slug_len);

    skip_space();
    if (content.count && *content.items != '\n') {
        blip.has_time = true;

        const char *time_begin = content.items;
        while (content.count && *content.items > ' ') adv();
        const size_t time_len = content.items - time_begin;
        assert(time_len == 5 && "Time format must be HH:MM");

        assert(true
            && '0' <= time_begin[0] && time_begin[0] <= '9'
            && '0' <= time_begin[1] && time_begin[1] <= '9'
            && time_begin[2] == ':'
            && '0' <= time_begin[3] && time_begin[3] <= '9'
            && '0' <= time_begin[4] && time_begin[4] <= '9'
        );

        blip.hour = (time_begin[0]-'0')*10 + (time_begin[1]-'0');
        blip.minute = (time_begin[0]-'0')*10 + (time_begin[1]-'0');

        assert(blip.hour < 24);
        assert(blip.minute < 60);

        skip_space();
    }

    assert(content.count && *content.items == '\n');
    adv();

    assert(content.count && *content.items == '\n' && "Expected blank line after header!");
    adv();

    assert(content.count && "Expected blip to have content");
    blip.content = strndup(content.items, content.count);

    return blip;
}

bool blip_lt(struct blip *a, struct blip *b) {
    if (a->year < b->year) return true;
    if (a->year > b->year) return false;
    if (a->month < b->month) return true;
    if (a->month > b->month) return false;
    if (a->day < b->day) return true;
    if (a->day > b->day) return false;
    if (a->index < b->index) return true;
    if (a->index > b->index) return false;
    assert(false && "two blips should not have the same date and index!");
    return false;
}

void blips_sort(struct blip *blips, size_t blips_count) {
    struct blip tmp;
    for (size_t i = 0; i < blips_count; ++i) {
        for (size_t j = i+1; j < blips_count; ++j) {
            if (blip_lt(&blips[j], &blips[i])) {
                tmp = blips[i];
                blips[i] = blips[j];
                blips[j] = tmp;
            }
        }
    }
    /*
    if (blips_count > 2) {
        blip_sort(blips, blips_count/2);
        blip_sort(blips + blips_count/2, blips_count - blips_count/2);

        struct blip *first = blips;
        struct blip *secnd = blips + blip_count/2;

        while (first < secnd && secnd < blips + blips_count) {
        }
    } else if (blips_count == 2) {
        if (blip_lt(&blips[1], &blips[0])) {
            tmp = blips[0];
            blips[0] = blips[1];
            blips[1] = tmp;
        }
    }
    */
}
}$
