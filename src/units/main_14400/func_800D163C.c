#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    u8 latched0[5][4];
    u8 pad14[0x2F];
    u8 pending43[5][4];
    s8 amounts57[5][4][5];
} Obj_800D163C;

void func_801E9F90(s32 arg0, u8 arg1);
void func_800D1758(Obj_800D163C *obj, u8 kind, s32 amount);

static inline void notify_latched(s32 rowOffset, s32 event) {
    func_801E9F90(rowOffset + event, 1);
}

/* Latch every pending slot, apply its five amounts, and return the mask of touched rows. */
u8 func_800D163C(Obj_800D163C *obj) {
    u8 mask = 0;
    s32 row;
    s32 col;
    s32 k;

    for (row = 0; ; row++) {
        s32 moreRows = row < 5;
        if (!moreRows) {
            break;
        }
        for (col = 0; ; col++) {
            u8 pending;
            s32 moreCols = col < 4;
            if (!moreCols) {
                break;
            }
            pending = obj->pending43[row][col];

            if (pending != 0) {
                mask |= 1 << row;
                obj->latched0[row][col] = pending;
                notify_latched(row * 4, col + 0x136);
                for (k = 0; ; k++) {
                    s8 amount;
                    s32 kind;
                    s32 moreAmounts = k < 5;
                    if (!moreAmounts) {
                        break;
                    }
                    amount = obj->amounts57[row][col][k];
                    kind = k - 0x17;

                    if (amount >= 0) {
                        func_800D1758(obj, kind, amount + 1);
                    }
                }
            }
        }
    }
    return mask;
}
