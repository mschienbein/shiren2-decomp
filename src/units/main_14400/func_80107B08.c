#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { char pad[8]; short delta; short index; void (*destroy)(void *, s32); } UnitVT;
typedef struct {
    Pos pos;
    char pad8[0x24 - 0x8];
    UnitVT *vt;
    char pad28[0x9A - 0x28];
    unsigned short f9A;
} Unit;
typedef struct { s32 index; s32 f4; } UnitIter;
extern unsigned char D_80142F20;
s32 func_800A9070(UnitIter *it, s32 kind);
Unit *func_800A910C(UnitIter *it);
s32 func_800A4520(void *self, void *target);
s32 func_80049CB4(s32 id, ...);
s32 func_800AA48C(u8 id, u8 sub);
char *func_800A3B20(Unit *u);
void func_800497F0(s32 message_id, ...);
Unit *func_80107760(s32 a, s32 b);
s32 func_800A3934(Unit *u);
s32 func_800A5D2C(void *obj, void *position, u16 flags);
s32 func_800A5AE8(Unit *u, Pos *pos);
void func_800A58FC(Unit *u, Pos *pos);
static inline void Pos_copy(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
s32 func_80107B08(Unit *u, void *b) {
    s32 count = 0;
    s32 msg;
    s32 flag;
    Pos pos;
    if (u->f9A & 0x40) {
        UnitIter it;
        it.index = 0;
        while (func_800A9070(&it, 0x52)) {
            if (func_800A910C(&it)->f9A & 0x40) {
                count++;
            }
        }
        if (count >= 5) {
            return func_800A4520(u, b) ^ 1;
        }
    }
    func_80049CB4(0x1131);
    func_80049CB4(6);
    msg = func_80049CB4(0x5C, u);
    func_80049CB4(7);
    flag = 0;
    if (func_800AA48C(0x52, 1) != 0) {
        flag = 1;
    } else {
        s32 other = D_80142F20 != 0x4F;
        if (!other) {
            flag = 1;
        }
    }
    if (flag) {
        return 1;
    }
    func_800497F0(0x155, msg, func_800A3B20(u));
    Pos_copy(&pos, &u->pos);
    if (u->f9A & 0x40) {
        count = 5 - count;
        if (count >= 4) {
            count = 3;
        }
    } else {
        count = 3;
    }
    for (;;) {
        Unit *clone;
        s32 placed;
        s32 failed;
        if (count-- <= 0) {
            break;
        }
        clone = func_80107760(1, 0);
        failed = func_800A3934(clone) != 1;
        if (!failed) {
            return 1;
        }
        if (u->f9A & 0x40) {
            placed = func_800A5D2C(clone, &pos, 2);
            clone->f9A |= 0x40;
        } else {
            placed = func_800A5AE8(clone, &pos);
        }
        if (placed) {
            func_80049CB4(6);
            func_80049CB4(0x14, clone, &pos);
            func_800A58FC(clone, &pos);
            func_80049CB4(7);
            continue;
        }
        if (clone != 0) {
            clone->vt->destroy((char *)clone + clone->vt->delta, 3);
        }
        break;
    }
    return 1;
}
