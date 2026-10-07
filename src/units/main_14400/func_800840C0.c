#include "common.h"

typedef unsigned short u16;

typedef struct {
    unsigned char pad0[0x12];
    u16 flags;
} Obj800840C0;

extern s32 D_801C33B4;

s32 func_8005B058(void);
void func_8005A670(s32 arg0, s32 arg1, s32 arg2);

void func_800840C0(Obj800840C0 *obj)
{
    s32 value;

    if (obj->flags & 1) {
        value = func_8005B058();
        if (obj->flags & 0x2000) {
            if (value != 4) {
                if (value < D_801C33B4) {
                    value = D_801C33B4;
                }
            } else {
                value = D_801C33B4;
            }
        } else {
            value = D_801C33B4;
        }
        func_8005A670(value, 0, 0);
    }
}
