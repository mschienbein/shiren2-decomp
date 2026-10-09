#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Position;
typedef Position Tmp800A46BC;
typedef struct { u8 value; } Dir;
typedef struct { u8 field00, kind01; } Item;
typedef struct Rect Rect;
typedef struct { u8 pad00[4]; Rect *rect04; u8 pad08[0x10]; } Zone;
extern u16 D_80143450[54][76];
extern Position *D_801476B8;
extern u16 D_8014767C;
extern Zone D_80143330[];
extern u8 D_80143448;
extern u32 func_800B1C6C(Position *pos);
extern s32 func_800D33FC(void *zone);
extern void *func_800A2594(Tmp800A46BC *out, void *arg, Dir cell);
extern void *func_800B1F90(void *pos);
/* B1F90 returns a room pointer; B2118 forwards it to B68DC for byte writes. */
extern void func_800B2118(Position *pos, void *room, s32 direction);
extern s32 func_80049CB4(s32 id, ...);
extern void *func_800B4D80(Position *pos);
extern s32 func_800B5BDC(Position *pos);
extern s32 func_800B38F8(Position *pos);
extern s32 func_800B4344(void);
extern s32 func_800A31C8(Rect *rect, Position *pos);

static __inline__ Dir *init_dir(Dir *dir, u8 value) {
    dir->value = value;
    return dir;
}

s32 func_800B43BC(Position *pos, s32 notify, u8 kind) {
    Position adjacent, neighbor;
    s32 bad = 0;
    s32 outside = 0;
    Position *point;
    Item *item;
    s32 before, after;
    if (pos->y >= 76 || pos->x >= 54 || pos->y < 0 || pos->x < 0) outside = 1;
    if (outside || !(func_800B1C6C(pos) & 0x4000) || (func_800B1C6C(pos) & 0x8020)) bad = 1;
    if (bad) return 0;
    D_80143450[pos->x][pos->y] &= ~0x4000;
    D_80143450[pos->x][pos->y] |= 0x200;
    if (!(func_800B1C6C(pos) & 0x1000)) {
        Dir direction4;
        func_800A2594(&adjacent, pos, *init_dir(&direction4, 4));
        point = &adjacent;
        if (func_800B1C6C(point) & 0x1000) {
            Dir direction4b;
            Position *next = &neighbor;
            func_800A2594(next, pos, *init_dir(&direction4b, 4));
            func_800B2118(pos, func_800B1F90(next), 0);
        }
        { Dir direction0;
          func_800A2594(point, pos, *init_dir(&direction0, 0)); }
        if (func_800B1C6C(point) & 0x1000) {
            Dir direction0b;
            Position *next = &neighbor;
            func_800A2594(next, pos, *init_dir(&direction0b, 0));
            func_800B2118(pos, func_800B1F90(next), 2);
        }
        { Dir direction2;
          func_800A2594(point, pos, *init_dir(&direction2, 2)); }
        if (func_800B1C6C(point) & 0x1000) {
            Dir direction2b;
            Position *next = &neighbor;
            func_800A2594(next, pos, *init_dir(&direction2b, 2));
            func_800B2118(pos, func_800B1F90(next), 3);
        }
        { Dir direction6;
          func_800A2594(point, pos, *init_dir(&direction6, 6)); }
        if (func_800B1C6C(point) & 0x1000) {
            Dir direction6b;
            Position *next = &neighbor;
            func_800A2594(next, pos, *init_dir(&direction6b, 6));
            func_800B2118(pos, func_800B1F90(next), 1);
        }
    }
    if (notify) {
    func_80049CB4(6);
    func_80049CB4(0x11F, pos, (u8)kind);
    func_80049CB4(7);
    item = func_800B4D80(pos);
    if (item && item->kind01 == 0xCF) func_80049CB4(0x127, 0x4D);
    func_80049CB4(0xD8, pos);
    adjacent.x = D_801476B8->x;
    {
    Position *current = &adjacent;
    current->y = D_801476B8->y;
    before = func_800B5BDC(current);
    func_800B38F8(pos);
    func_800B4344();
    after = func_800B5BDC(current);
    }
    if (!before && after && !(D_8014767C & 0xC)) {
        s32 i = 0;
        Zone *zone = D_80143330;
        for (;;) {
            if (i >= D_80143448) return 1;
            if (zone->rect04 && func_800A31C8(zone->rect04, &adjacent)) {
                func_800D33FC(zone);
                return 1;
            }
            zone++;
            i++;
        }
    }
    }
    return 1;
}
