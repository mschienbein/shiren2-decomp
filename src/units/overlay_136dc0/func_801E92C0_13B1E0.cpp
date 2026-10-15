#include "common.h"
typedef unsigned char u8;
/* Four-byte records shared with func_801E9064_13AF84; every field is
 * accessed with lbu/sb and a merged record is cleared with size 4. */
typedef struct { u8 mask, row, column, active; } S06Selection;
#ifdef __cplusplus
extern "C" {
#endif
extern void *func_800C9E00(void);
extern u8 func_801E99C4_13B8E4(u8 row, s32 mode);
extern void func_801E9AFC_13BA1C(u8 row, u8 value);
extern void func_800D1B4C(u8 *grid, s32 row, s32 column);
extern void func_800265E0(void *dst, s32 size);

void func_801E92C0_13B1E0(S06Selection *records, u8 count) {
    u8 *grid = (u8 *)func_800C9E00();
    s32 first = func_801E99C4_13B8E4(0, 0);
    s32 second = func_801E99C4_13B8E4(1, 0);
    s32 i, j;
    for (i = 0; i < count; i++) {
        if (records[i].active == 1) func_800D1B4C(grid, records[i].row, records[i].column);
    }
    for (i = 0; i < count; i++) {
        for (j = i + 1; j < count; j++) {
            if (records[j].mask && records[i].row == records[j].row && records[i].column == records[j].column) {
                records[i].mask |= records[j].mask;
                records[i].active |= records[j].active;
                func_800265E0(&records[j], 4);
            }
        }
    }
    for (i = 0; i < count; i++) {
        if (records[i].mask && records[i].active) {
            func_801E9AFC_13BA1C(records[i].row, func_801E99C4_13B8E4(records[i].row, 0) - 1);
        }
    }
    for (i = 0; i < count; i++) {
        if (records[i].row == 0) {
            if (first == 4) records[i].column = 3;
            else records[i].column = 0;
        } else if (records[i].row == 1) {
            if (second == 4) records[i].column = 3;
            else records[i].column = 0;
        }
    }
    for (i = 0; i < count; i++) {
        for (j = i + 1; j < count; j++) {
            if (records[j].mask && records[i].row == records[j].row && records[i].column == records[j].column) {
                records[i].mask |= records[j].mask;
                func_800265E0(&records[j], 4);
            }
        }
    }
}
#ifdef __cplusplus
}
#endif
