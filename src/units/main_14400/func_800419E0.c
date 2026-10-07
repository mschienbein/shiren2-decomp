#include "common.h"

typedef unsigned short u16;

typedef struct {
    char pad0[0xE4];
    u16 unkE4;
    char padE6[0x104 - 0xE6];
    s32 unk104;
} Obj800419E0;

extern Obj800419E0 *D_801476B8;

s32 func_800419E0(void) {
    Obj800419E0 *obj = D_801476B8;
    s32 result = 0;

    if ((obj->unkE4 >> 3) & 1) {
        result = obj->unk104 == 0;
    }
    return result;
}
