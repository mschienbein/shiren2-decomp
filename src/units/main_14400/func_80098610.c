#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 unk0;
    s32 unk4;
} Pair80098610;

typedef struct {
    char pad0[0x20];
    s32 unk20;
    char pad24[0x34 - 0x24];
    Pair80098610 pos;
    char pad3C[0x2FC - 0x3C];
    s32 mode;
} Obj80098610;

extern u8 *func_8006A810(void *dst, s32 value, s32 count);
extern void func_800487C4(Obj80098610 *obj);
extern s32 func_80098E34(Obj80098610 *obj, s32 offset);
extern void func_800488F0(Obj80098610 *obj, Pair80098610 *cur, s32 arg2, Pair80098610 *prev);

void func_80098610(Obj80098610 *obj, Pair80098610 *pos) {
    Pair80098610 cur;
    Pair80098610 prev;
    s32 oldY;

    if (obj->mode == 8 || obj->mode == 16) {
        return;
    }
    func_8006A810(&prev, 0, sizeof(prev));
    prev.unk0 = pos->unk0;
    cur = prev;
    {
        Pair80098610 tmp;

        func_8006A810(&tmp, 0, sizeof(tmp));
        tmp.unk0 = obj->pos.unk0;
        prev = tmp;
    }
    oldY = obj->pos.unk4;
    obj->pos = *pos;
    pos = &obj->pos;
    if (obj->pos.unk4 != oldY) {
        func_800487C4(obj);
    }
    if (obj->mode == 0x400 || func_80098E34(obj, obj->pos.unk0 + obj->unk20 * pos->unk4) < 0) {
        cur.unk0 = -1;
    }
    func_800488F0(obj, &cur, 0, &prev);
}
