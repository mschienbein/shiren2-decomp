#include "map_rect.h"
#include "map_rect_presets.h"
inline void *operator new(unsigned int, void *p) { return p; }

static inline void copy_rect(MapRect *dst, const MapRect *src) {
    dst->min = src->min;
    dst->max = src->max;
}
/* Copy one s32 member through references to that member.  Every access stays
   inside its named field.  Inlined at RTL level, the reads are MEMs through the
   reference parameter rather than through an aggregate COMPONENT_REF, so
   sched.c:830-856 true_dependence lets the D_801474B0 loads move ahead of the
   preceding D_801474B0[5] store, as at VRAM 0x800BA3E0..0x800BA414. */
static inline void copy_word(s32 &dst, const s32 &src) { dst = src; }
static inline void copy_presets(MapRectPresets *dst, const MapRect *src) {
    copy_word(dst->entries[0].min_x, src[0].min.x);
    copy_word(dst->entries[0].min_y, src[0].min.y);
    copy_word(dst->entries[0].max_x, src[0].max.x);
    copy_word(dst->entries[0].max_y, src[0].max.y);
    copy_word(dst->entries[1].min_x, src[1].min.x);
    copy_word(dst->entries[1].min_y, src[1].min.y);
    copy_word(dst->entries[1].max_x, src[1].max.x);
    copy_word(dst->entries[1].max_y, src[1].max.y);
    copy_word(dst->entries[2].min_x, src[2].min.x);
    copy_word(dst->entries[2].min_y, src[2].min.y);
    copy_word(dst->entries[2].max_x, src[2].max.x);
    copy_word(dst->entries[2].max_y, src[2].max.y);
    copy_word(dst->entries[3].min_x, src[3].min.x);
    copy_word(dst->entries[3].min_y, src[3].min.y);
    copy_word(dst->entries[3].max_x, src[3].max.x);
    copy_word(dst->entries[3].max_y, src[3].max.y);
    copy_word(dst->entries[4].min_x, src[4].min.x);
    copy_word(dst->entries[4].min_y, src[4].min.y);
    copy_word(dst->entries[4].max_x, src[4].max.x);
    copy_word(dst->entries[4].max_y, src[4].max.y);
    copy_word(dst->entries[5].min_x, src[5].min.x);
    copy_word(dst->entries[5].min_y, src[5].min.y);
    copy_word(dst->entries[5].max_x, src[5].max.x);
    copy_word(dst->entries[5].max_y, src[5].max.y);
}

extern "C" MapRect D_801474A0;
/* The six source rectangles are MapRect objects: the original writes each
   entry from the local MapRect as two 8-byte MapPos corner copies (lw s4/s5 from
   sp+0/4 then sp+8/12, two word stores each, VRAM 0x800BA290..0x800BA3DC), the
   same code as the D_801474A0 copy.  Scalar MapRectPreset fields are stored as
   CSE'd constants instead, which also forwards them into the final copy (536 B,
   not the original 848 B).  Only D_80147510, read by func_800B8628 as plain
   records, is a MapRectPresets. */
extern "C" MapRect D_801474B0[6];
extern "C" MapRectPresets D_80147510;

extern "C" void func_800BA208(void) {
    MapRect rect;
    /* Keep the maximum corner addressed through its own pointer, as in the
       original addiu v1,sp,8 and sw ...,4(v1). */
    MapPos *maximum = &rect.max;

    rect.min.x = 13;
    rect.min.y = 25;
    maximum->x = 20;
    maximum->y = 29;
    copy_rect(&D_801474A0, &rect);
    rect.min.x = 0;
    rect.min.y = 0;
    maximum->x = 33;
    maximum->y = 27;
    copy_rect(&D_801474B0[0], &rect);
    rect.min.x = 0;
    rect.min.y = 28;
    maximum->x = 33;
    maximum->y = 55;
    copy_rect(&D_801474B0[1], &rect);
    rect.min.x = 0;
    rect.min.y = 0;
    maximum->x = 16;
    maximum->y = 27;
    copy_rect(&D_801474B0[2], &rect);
    rect.min.x = 17;
    rect.min.y = 0;
    maximum->x = 33;
    maximum->y = 27;
    copy_rect(&D_801474B0[3], &rect);
    rect.min.x = 0;
    rect.min.y = 28;
    maximum->x = 16;
    maximum->y = 55;
    copy_rect(&D_801474B0[4], &rect);
    rect.min.x = 17;
    rect.min.y = 28;
    maximum->x = 33;
    maximum->y = 55;
    copy_rect(&D_801474B0[5], &rect);
    /* Default-initialize genuine POD array storage, then populate its fields.
       Unlike MapRect[], this record array has no native member constructors. */
    copy_presets(new (&D_80147510) MapRectPresets, D_801474B0);
}
