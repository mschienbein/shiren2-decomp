#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 field_0;
    s32 active;
} Link;

typedef struct {
    s32 x;
    s32 y;
} Point;

typedef struct {
    u8 pad0[0x1E];
    u8 flags1E;
    u8 pad1F[0x54 - 0x1F];
    u8 flags54;
    u8 pad55[0x64 - 0x55];
    Point pos;
    u8 pad6C[0x72 - 0x6C];
    u8 flags72;
    u8 pad73[0x78 - 0x73];
    Link linkB;
    u8 pad80[0x84 - 0x80];
    Link linkA;
} Obj;

/* The caller supplies its action receiver even though this operation does not use it. */
void func_800DF61C(void *arg0, Obj *obj, Point *pos) {
    s32 linked = 0;

    if (((obj->flags1E >> 3) & 1)
        && ((obj != 0) ? &obj->linkA : 0)->active != 0) {
        linked = 1;
    } else if (((obj->flags1E >> 6) & 1)
               && ((obj != 0) ? &obj->linkB : 0)->active != 0) {
        linked = 1;
    }
    if (linked) {
        obj->pos = *pos;
    }
    obj->flags54 |= 2;
    obj->flags72 |= 4;
}
