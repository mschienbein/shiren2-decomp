#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
/* Ten 0x2C-byte records. func_80081C18 writes separate halfwords at +0/+2
 * (0x80081C6C/70); this view does not use either field. */
typedef struct { u8 pad_00[4]; u16 field_04, field_06, field_08, field_0A, field_0C; u8 pad_0E[0x1E]; } Room;
extern u16 D_801A906C[10];
extern u8 D_801A8BA0[30][40];
extern s32 D_8013E814, D_8013E818;
extern u16 D_801A9058[10];
extern Room D_801A9080[10];
void func_80081964(void)
{
    s32 i, x, y;
    s32 min_x, min_y, max_x, max_y;
    s32 offset;
    for (i = 9, offset = 18; i >= 0; i--, offset -= 2) {
        *(u16 *)((u8 *)D_801A906C + offset) = 0;
    }
    for (y = 0; y < 30; y++) {
        s32 cell;
        for (x = 39, cell = y * 40 + x; x >= 0; x--, cell--) {
            ((u8 *)D_801A8BA0)[cell] = 15;
        }
    }
    for (i = D_8013E814; i < D_8013E818; i++) {
        s32 number = D_801A9058[i];
        Room *room = &D_801A9080[number];
        if (room->field_04 == 0) {
            min_y = room->field_08;
            min_x = room->field_06;
            max_y = min_y + room->field_0C;
            max_x = min_x + room->field_0A;
        } else {
            min_y = room->field_08 - 1;
            min_x = room->field_06 - 1;
            max_y = min_y + room->field_0C + 2;
            max_x = min_x + room->field_0A + 2;
        }
        for (y = min_y; y < max_y; y++) {
            for (x = min_x; x < max_x; x++) {
                u8 old = D_801A8BA0[y][x];
                u8 flags = (x == min_x) << 4;
                if (x == max_x - 1) flags |= 0x20;
                if (y == min_y) flags |= 0x40;
                if (y == max_y - 1) flags |= 0x80;
                if (old == 15) {
                    if (flags != 0x10 && flags != 0x20 && flags != 0x40 && flags != 0x80) {
                        flags = 0;
                    }
                    D_801A8BA0[y][x] = flags | number;
                } else {
                    /* The original compares against min_x - 1 here, not max_x - 1. */
                    if (D_801A906C[old & 15] == 0 && y != min_y && y != max_y - 1 && x != min_x && x != min_x - 1) {
                        D_801A906C[old & 15] = 1;
                    }
                    if ((x == min_x && (y == min_y || y == max_y - 1)) ||
                        (x == max_x - 1 && (y == min_y || y == max_y - 1))) {
                        old &= flags;
                    } else {
                        old = flags;
                    }
                    D_801A8BA0[y][x] = old | number;
                }
            }
        }
    }
}
