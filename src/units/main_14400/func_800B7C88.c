#include "common.h"
#include "map_rect.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 value; } Dir;
extern u8 D_80147620[];
extern s32 func_800C5B20(void *rng, u16 alignment);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern u32 func_800B1C6C(MapPos *pos);
extern void *func_800A2594(MapPos *out, void *arg, Dir cell);
extern s32 func_800B43BC(MapPos *pos, s32 mode, u8 flag);
extern s32 func_800B38F8(MapPos *pos);

static inline void set_pair(MapPos *p, s32 x, s32 y) {
    p->x = x;
    p->y = y;
}
static inline void copy_pair(MapPos *dst, MapPos *src) {
    *dst = *src;
}
static inline void copy_rect(MapRect *dst, MapRect *src) {
    copy_pair(&dst->min, &src->min);
    copy_pair(&dst->max, &src->max);
}
/* ODD_C: the original's tiny accessors (attempt counter, lower bound, direction
   range test) are kept as inline helpers; they also shape the loop tests and the
   full-word read of the lower bound. */
static inline s32 remaining(s32 count) { return count; }
static inline s32 lower_y(MapRect *bounds) { return bounds->min.y; }
static inline s32 direction_valid(s32 i) { return i < 8; }
static inline Dir *set_direction(Dir *dir, u8 value) {
    dir->value = value;
    return dir;
}

void func_800B7C88(void *kind, const MapRect *a, const MapRect *b) {
    MapRect bounds;
    MapRect *range;
    /* Copy candidate into bounds before selecting pos. The search phase keeps
     * neighbor and first_path separate; second_path replaces search only after
     * the first corridor call returns. No inactive union view is read. */
    union {
        MapRect candidate;
        struct {
            MapPos pos;
            union {
                struct { MapPos neighbor; MapRect first_path; } search;
                MapRect second_path;
            } scratch;
        } selected;
    } work;
    Dir direction;
    s32 attempts;
    if (func_800C5B20(D_80147620, 2)) {
        set_pair(&work.candidate.min, a->min.y, b->min.x);
        set_pair(&work.candidate.max, a->max.y, b->max.x);
        copy_rect(&bounds, &work.candidate);
    } else {
        set_pair(&work.candidate.min, b->min.y, a->min.x);
        set_pair(&work.candidate.max, b->max.y, a->max.x);
        copy_rect(&bounds, &work.candidate);
    }
    attempts = 100;
    while (remaining(--attempts) != -1) {
        s32 nearby;
        s32 i;
        range = &bounds;
        work.selected.pos.x = (u8)func_800C5844(D_80147620, lower_y(range), range->max.y);
        work.selected.pos.y = (u8)func_800C5844(D_80147620, bounds.min.x, range->max.x);
        if (func_800B1C6C(&work.selected.pos) & 0x1000) continue;
        nearby = 0;
        for (i = 0; ; i++) {
            if (!direction_valid(i)) break;
            func_800A2594(&work.selected.scratch.search.neighbor, &work.selected.pos, *set_direction(&direction, i & 7));
            if (func_800B1C6C(&work.selected.scratch.search.neighbor) & 0x1000) {
                nearby = 1;
                break;
            }
        }
        if (!nearby) break;
    }
    func_800B43BC(&work.selected.pos, 0, 0);
    func_800B38F8(&work.selected.pos);
    work.selected.scratch.search.first_path.min = work.selected.pos;
    work.selected.scratch.search.first_path.max = work.selected.pos;
    func_800B7E9C(kind, &work.selected.scratch.search.first_path, a);
    work.selected.scratch.second_path.min = work.selected.pos;
    work.selected.scratch.second_path.max = work.selected.pos;
    func_800B7E9C(kind, &work.selected.scratch.second_path, b);
}
