#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Vec2i;
typedef struct { Vec2i start; Vec2i end; } Rect;
typedef struct { Vec2i cur; Vec2i start; Vec2i end; } RectIter;
typedef struct { u8 unk0; u8 kind; } Thing;
/* 0x10-byte state: func_800D459C stores room at +0 and words at +4/+8/+C. */
typedef struct { void *room; s32 opaque[3]; } RoomState;
extern u8 D_80143391;
extern u8 D_80143392;
extern Rect D_801429C0;
extern u16 D_80143450[][76];
extern u8 D_8014344C;
extern u8 D_80143448;
extern s32 D_80143444;
extern u16 D_8014767C;
extern char D_801431F0[];
extern char D_80143330[];
extern RoomState D_80143434;
void *func_800A3610(void *out, void *it);
u32 func_800B1C6C(void *pos);
Thing *func_800B4D80(Vec2i *);
void func_800B2884(void);
void func_800B6728(void *, Rect *);
void func_800D3640(void *, s32);
void func_800D3C68(void);
s32 func_800B60D0(void);
void func_800D49B8(void **p, void *v);
s32 func_80049CB4(s32 id, ...);
void func_800B4A70(Rect *);
static inline void rect_iter_set_start(RectIter *it, s32 x, s32 y) {
    Vec2i v;
    v.x = x;
    v.y = y;
    it->start = v;
    it->cur = it->start;
}
static inline void rect_iter_set_end(RectIter *it, s32 x, s32 y) {
    Vec2i v;
    v.x = x;
    v.y = y;
    it->end = v;
}
static inline s32 rect_iter_valid(RectIter *it) {
    return it->cur.x <= it->end.x;
}
void func_800B23B0(void) {
    RectIter iter;
    s32 found = 0;
    D_80143392 = 0;
    D_80143391 |= 4;
    rect_iter_set_start(&iter, D_801429C0.start.x, D_801429C0.start.y);
    rect_iter_set_end(&iter, D_801429C0.end.x, D_801429C0.end.y);
    while (rect_iter_valid(&iter)) {
        Vec2i pos;
        func_800A3610(&pos, &iter);
        if (func_800B1C6C(&pos) & 0x4000) {
            Thing *thing = func_800B4D80(&pos);
            if (thing != 0 && thing->kind == 0xCF) {
                found = 1;
            }
        }
        D_80143450[pos.x][pos.y] &= 0x9700;
        D_80143450[pos.x][pos.y] |= 0x1600;
    }
    func_800B2884();
    D_8014344C = 1;
    func_800B6728(D_801431F0, &D_801429C0);
    if (D_80143448) {
        func_800D3640(D_80143330, 0);
        func_800D3C68();
        D_80143448 = 1;
    }
    if (func_800B60D0()) {
        func_800D49B8(&D_80143434.room, D_801431F0);
    } else if (D_80143444 != 0 && !(D_8014767C & 0xC)) {
        func_80049CB4(0x126, 0x1F);
    }
    if (found) {
        func_80049CB4(0x127, 0x4D);
    }
    func_800B4A70(&D_801429C0);
    func_80049CB4(0xDB);
}
