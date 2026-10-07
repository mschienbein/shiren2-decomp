#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x2];
    s16 kind;
    u8 pad4[0x3D];
    u8 level;
    u8 pad42[0xA];
    u8 *stats;
    u8 pad50[0x60];
} Entry;

extern Entry D_801DEAB4[];

void func_8007482C(s32 index, s32 value)
{
    Entry *entry = &D_801DEAB4[index];
    s32 level;
    s32 max;
    u8 result;

    if (D_801DEAB4[index].kind == 0x29) {
        if (value < 20) {
            level = 0;
        } else if (value < 30) {
            level = 1;
        } else if (value < 60) {
            level = 2;
        } else if (value < 70) {
            level = 3;
        } else if (value < 77) {
            level = 4;
        } else if (value < 88) {
            level = 5;
        } else if (value < 90) {
            level = 6;
        } else if (value < 99) {
            level = 7;
        } else {
            level = 8;
        }
    } else {
        level = 0;
        if (value > 0) {
            level = value - 1;
        }
    }

    max = *entry->stats & 0x1F;
    if (level < max) {
        result = level;
    } else {
        result = max - 1;
    }
    entry->level = result;
}
