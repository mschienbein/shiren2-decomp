#include "common.h"
#include "map_types.h"

/*
 * Single-room-chain layout pass of the floor generator (C++ TU). It is the
 * entry-13 pass of the pointer-to-member table D_80153CAC dispatched by
 * func_800BA854.cpp. Rooms are stored from entry 1 by func_800C012C, then
 * shifted down by one entry, translated by D_801C9BC0 (lower corner + delta,
 * upper corner - delta) and exported to D_801431F0.
 *
 * Each translated corner is computed into the function-scope `corner`
 * (sp+0x18, the slot released by the first loop's MapPos(4, 4) limit) by the
 * out-pointer helpers func_800A2544 / func_800A256C and then copied into the
 * room. The copy is written as one assignment whose right operand is the
 * comma expression *(helper(&corner, ...), &corner): because the left-hand
 * room corner is a component reference, expand_assignment computes its address
 * (floor + i*20) before the helper call. That address is then the first giv of
 * i that loop.c records, so the reductions run table, source offset,
 * destination: the preheader sets s5/s4/s1 in that order and the loop tail
 * advances s5, s4, then i, with the destination advance in the branch delay
 * slot. The value read is `corner` itself, not the helper's returned pointer.
 */

typedef unsigned char u8;

struct Rect_800BFEA0 {
    MapPos tl;
    MapPos br;
};

struct Entry_800BFEA0 {
    Rect_800BFEA0 rect;
    u8 tag[4];
};

/* Partial view of the generator: room count at 0x3DC, entries at 0x404 (func_800BA6A0.cpp). */
struct Floor_800BFEA0 {
    u8 pad_000[0x3DC];
    s32 room_count;
    u8 pad_3E0[0x404 - 0x3E0];
    Entry_800BFEA0 rooms[17];
};

extern "C" {
extern u8 D_80147620[];
extern MapPos D_801C9BB0;
extern MapPos D_801C9BB8;
extern MapPos D_801C9BC0;
extern Entry_800BFEA0 D_801431F0[];
extern u8 D_8014344C;
extern u8 D_80143392;
void func_800B7884(void *generator);
s32 func_800C5844(void *rng, u8 base, u8 top);
s32 func_800C012C(void *floor, Rect_800BFEA0 *out, const MapPos &from, const MapPos &to, u8 count);
MapPos *func_800A2544(MapPos *out, MapPos *a, MapPos *b);
MapPos *func_800A256C(MapPos *out, MapPos *a, MapPos *b);
void func_800C0304(Floor_800BFEA0 *floor);
void func_800C056C(Floor_800BFEA0 *floor);
void func_800C07FC(Floor_800BFEA0 *floor);
void func_800B17A4(void);
}

extern "C" void func_800BFEA0(Floor_800BFEA0 *floor)
{
    func_800B7884(floor);
    floor->rooms[0].rect.tl = D_801C9BB0;
    floor->rooms[0].rect.br = D_801C9BB8;
    floor->room_count = 1;
    u8 wanted = func_800C5844(D_80147620, 5, 0x10);
    for (s32 tries = 0; tries < 100; tries++) {
        if (func_800C012C(floor, &floor->rooms[floor->room_count].rect, MapPos(4, 4), MapPos(0x10, 0x10), 0x14)) {
            if (++floor->room_count == wanted + 1) {
                break;
            }
        }
    }
    floor->room_count--;
    MapPos corner;
    for (s32 i = 0; i < floor->room_count; i++) {
        floor->rooms[i].rect.tl = *(func_800A2544(&corner, &floor->rooms[i + 1].rect.tl, &D_801C9BC0), &corner);
        floor->rooms[i].rect.br = *(func_800A256C(&corner, &floor->rooms[i + 1].rect.br, &D_801C9BC0), &corner);
        D_801431F0[i] = floor->rooms[i];
    }
    D_8014344C = floor->room_count;
    func_800C0304(floor);
    func_800C056C(floor);
    func_800C07FC(floor);
    func_800B17A4();
    D_80143392 = 0;
}
