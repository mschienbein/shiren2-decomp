#include "func_801E9064_13AF84/types.h"
extern void *func_800C9E00(void);
extern u8 D_80147620[];
extern u8 func_800C57CC(void *rng, s32 range);
extern s32 func_801E9C6C_13BB8C(void);
extern u8 func_801E99C4_13B8E4(u8 row, s32 mode);
extern u32 func_801E8F90_13AEB0(u8 *slots, u8 value);
void func_801E9064_13AF84(S06Selection *records, u8 *slots, u8 *codes, u8 count) {
    u8 *grid = func_800C9E00();
    s32 same = func_800C57CC(D_80147620, 1);
    s32 i, j;
    for (i = 0; i < count; i++) {
        s32 row = func_801E9C6C_13BB8C();
        s32 columns = func_801E99C4_13B8E4(row, 0);
        s32 column = 0;
        if (columns >= 2) column = func_800C57CC(D_80147620, (u8)(columns - 1));
        records[i].row = row;
        records[i].column = column;
        records[i].mask = func_801E8F90_13AEB0(slots, 0);
        if (same == 1) {
            records[i].row = records[0].row;
            records[i].column = records[0].column;
        }
    }
    for (i = 0; ; i++) {
        u8 threshold;
        s32 cell;
        if (i >= count) break;
        cell = records[i].column;
        cell += records[i].row * 4;
        threshold = grid[cell];
        if (threshold < func_800C57CC(D_80147620, 100) && threshold) {
            records[i].active = 1;
            codes[i] = records[i].column + records[i].row * 4 + 1;
        }
    }
    for (i = 0; i < count; i++) {
        for (j = 1; j < count; j++) {
            if (i == j) continue;
            if (codes[i] == codes[j]) codes[j] = 0;
        }
    }
    for (i = 0; ; i++) {
        u8 *current;
        if (i >= count) break;
        j = 1;
        current = &codes[i];
        for (; ; j++) {
            s32 swap;
            if (j >= count) break;
            if (i == j) continue;
            if (codes[j] < *current) {
                swap = *current;
                *current = codes[j];
                codes[j] = swap;
            }
        }
    }
}
