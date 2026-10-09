#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xC];
    u8 flags_C;
    u8 kind_D;
    u8 level_E;
    u8 count_F;
    u8 field_10;
} Item800D7B04;

extern u8 D_80148190[];
/* Level thresholds (index 1..8) and the level each band maps to (9 entries). */
extern u8 D_801480D0[];
extern u8 D_801480F4[];

s32 func_800D7D84(u8 kind, u8 level);
void *func_80128360(void);
s32 func_800AC670(void *key);
char *func_80044FDC(u8 a, u8 b);

/* Both callers' values arrive zero-extended: func_800D7A24 passes its u8 kind/level. */
void *func_800D7B04(u8 kind, u8 level) {
    s32 index = func_800D7D84(kind, level);
    Item800D7B04 *item;
    s32 i;
    s32 ok;

    if (index == -1) {
        return 0;
    }
    if (D_80148190[index] == 0) {
        return 0;
    }
    item = func_80128360();
    ok = func_800AC670(item) != 1;
    if (ok) {
        if (kind == 0x29) {
            for (i = 1; i < 9; i++) {
                if (level < D_801480D0[i]) {
                    break;
                }
            }
            level = D_801480F4[i - 1];
        }
        item->kind_D = kind;
        item->level_E = level;
        item->count_F = D_80148190[index] - 1;
        item->flags_C |= 6;
        item->field_10 = func_80044FDC(kind, level)[0x13];
        return item;
    }
    return 0;
}
