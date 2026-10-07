#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} Entry800D80B0;

void *func_80044ECC(u8 kind, u8 level);
s32 func_800D8088(u8 id, u8 index);

s32 func_800D80B0(u8 id) {
    Entry800D80B0 *entry = func_80044ECC(id, 1);
    s32 i;

    if (entry != 0) {
        i = entry->unk2;
        while (i > 0) {
            if (func_800D8088(id, i)) {
                return 1;
            }
            i--;
        }
    }
    return 0;
}
