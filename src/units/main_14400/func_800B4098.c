#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos800B4098;
typedef struct { Pos800B4098 tl; Pos800B4098 br; u8 pad10[4]; } Room800B4098;
typedef struct { Pos800B4098 cur; Pos800B4098 start; Pos800B4098 end; } RectIter800B4098;
/* One of the two 0x18-byte region records initialized by func_800B1080: owner byte +0
 * (func_800D1D90 clears it with sb zero,0(a0)) plus 3 padding bytes, room pointer +4. */
typedef struct { s8 owner; u8 pad1[3]; Room800B4098 *room; u8 pad8[0x10]; } Ent800B4098;
extern Room800B4098 D_801431F0[];
extern Ent800B4098 D_80143330[2];
typedef struct { void *room; s32 opaque[3]; } RoomState;
extern RoomState D_80143434;
extern u8 D_80143448;
extern u16 D_80143450[][0x4C];
Pos800B4098 *func_800A3610(Pos800B4098 *out, RectIter800B4098 *it);
void func_800B6590(Room800B4098 *room, Pos800B4098 *tl, Pos800B4098 *br);
void func_800D28D8(Ent800B4098 *ent);
void func_800B1820(s32 id);
static inline Room800B4098 *state_room(RoomState *state) { return state->room; }
static inline void set_state_room(RoomState *state, Room800B4098 *room) { state->room = room; }
void func_800B4098(u8 idA, u8 idB) {
    Room800B4098 *a = &D_801431F0[idA];
    Room800B4098 *b = &D_801431F0[idB];
    s32 ax, ay, axe, aye;
    s32 bx, by, bxe, bye;
    s32 m;
    s32 v;
    Pos800B4098 tl;
    Pos800B4098 br;
    RectIter800B4098 it;
    Room800B4098 *merged;
    Room800B4098 *dst;
    s32 i;
    u8 lo;
    u8 hi;
    u8 flag;
    ay = a->tl.y;
    by = b->tl.y;
    ax = a->tl.x;
    aye = a->br.y;
    axe = a->br.x;
    bx = b->tl.x;
    bye = b->br.y;
    bxe = b->br.x;
    m = ay;
    if (by < m) {
        m = by;
    }
    tl.y = m;
    v = ax;
    if (bx < v) {
        v = bx;
    }
    tl.x = v;
    v = aye;
    if (v < bye) {
        v = bye;
    }
    br.y = v;
    v = axe;
    if (v < bxe) {
        v = bxe;
    }
    br.x = v;
    lo = idB;
    if (idA < idB) {
        lo = idA;
    }
    hi = idB;
    if (idB < idA) {
        hi = idA;
    }
    flag = lo & 0xF;
    it.start = tl;
    it.cur = it.start;
    it.end = br;
    for (;;) {
        Pos800B4098 cell;
        s32 more = it.cur.x <= it.end.x;
        if (!more) {
            break;
        }
        func_800A3610(&cell, &it);
        D_80143450[cell.x][cell.y] &= 0xF7F0;
        D_80143450[cell.x][cell.y] = flag | (D_80143450[cell.x][cell.y] | 0x1200);
    }
    merged = &D_801431F0[lo];
    func_800B6590(merged, &tl, &br);
    for (i = 0, dst = merged; ; i++) {
        Ent800B4098 *e;
        if (i >= D_80143448) {
            break;
        }
        e = &D_80143330[i];
        if (e->room == a || e->room == b) {
            e->room = dst;
            func_800D28D8(e);
        }
    }
    if (state_room(&D_80143434) == a || state_room(&D_80143434) == b) {
        set_state_room(&D_80143434, &D_801431F0[lo]);
    }
    func_800B1820(hi);
}
