#include "common.h"

typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 field_0;
    s32 field_4;
    s32 field_8;
} Triple;

typedef struct {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_C;
} Quad;

/* Number-entry dialog (see func_800A1A64): title text id at +0x6C, running total at
 * +0x70, bounds at +0x84. */
typedef struct {
    char pad0[0x6C];
    s16 title_6C;
    char pad6E[2];
    u32 total_70;
    char pad74[0x10];
    Quad bounds_84;
} Work;

extern char *func_80048480(u16 id);
extern void func_8009775C(Work *w, char *label, char *text, s32 c, s32 d, s32 e, s32 f, Triple *layout);

void func_8009F310(Work *work, u32 maximum, s32 digits, u32 value, Triple layout, s32 title, u32 total, Quad bounds)
{
    func_8009775C(work, func_80048480(0x1F3), func_80048480(0x1F4), 0, maximum, digits, value, &layout);
    work->title_6C = title;
    work->bounds_84 = bounds;
    work->total_70 = total;
}
