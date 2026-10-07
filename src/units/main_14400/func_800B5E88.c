#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pos800B5E88;

typedef struct {
    u8 pad0[0x4];
    void *area_4;
    u8 pad8[0xC];
    s32 field_14;
} Zone800B5E88;

typedef struct {
    Pos800B5E88 pos_0;
} Unit800B5E88;

extern s32 D_80143444;
extern u16 D_8014767C;
extern u8 D_80143391;
extern u8 D_80143448;
extern Zone800B5E88 D_80143330[];
extern Unit800B5E88 *D_801476B8;

s32 func_800A31C8(void *area, Pos800B5E88 *pos);
s32 func_800D3510(void *zone);
s32 func_800D33FC(void *zone);
s32 func_800D2FB0(Zone800B5E88 *zone);
s32 func_800D2A64(Zone800B5E88 *zone);

Pos800B5E88 D_80143388 = { 0, 0 };

void func_800B5E88(void) {
    Pos800B5E88 cur;
    s32 skip = 0;
    s32 wasInside;
    s32 isInside;
    s32 oldIndex;
    s32 newIndex;
    s32 leave;
    s32 i;
    s32 j;
    Zone800B5E88 *zone;

    if (D_80143444 == 0) {
        skip = 1;
    } else if (D_8014767C & 0xC) {
        skip = 1;
    } else if (D_80143391 & 4) {
        skip = 1;
    } else if (D_80143391 & 8) {
        skip = 1;
    }
    wasInside = 0;
    if (skip) {
        return;
    }
    oldIndex = -1;
    cur.x = D_801476B8->pos_0.x;
    cur.y = D_801476B8->pos_0.y;
    for (i = 0, zone = D_80143330;; zone++, i++) {
        s32 count = D_80143448;

        if (i >= count) {
            break;
        }
        if (func_800A31C8(zone->area_4, &D_80143388)) {
            wasInside = 1;
            oldIndex = i;
            break;
        }
    }
    isInside = 0;
    newIndex = -1;
    for (i = 0, zone = D_80143330;; zone++, i++) {
        s32 count = D_80143448;

        if (i >= count) {
            break;
        }
        if (func_800A31C8(zone->area_4, &cur)) {
            isInside = 1;
            newIndex = i;
            break;
        }
    }
    if (wasInside && (!isInside || newIndex != oldIndex)) {
        func_800D3510(&D_80143330[oldIndex]);
    }
    if (D_8014767C & 0xC) {
        return;
    }
    if (isInside && newIndex != oldIndex) {
        func_800D33FC(&D_80143330[newIndex]);
    }
    for (j = 0;; j++) {
        s32 count = D_80143448;
        Zone800B5E88 *z = &D_80143330[j];

        if (j >= count) {
            break;
        }
        leave = 0;
        if (func_800D2FB0(z) && (func_800D2A64(z) || z->field_14 != 0) && !func_800A31C8(z->area_4, &cur)) {
            leave = 1;
        }
        if (leave) {
            func_800D3510(z);
        }
    }
    D_80143388 = cur;
}
