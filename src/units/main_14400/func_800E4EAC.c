#include "common.h"

typedef unsigned char u8;
typedef signed char s8;

typedef struct {
    u8 pad0[0x34];
    u8 field_34;
    u8 pad35[0x21];
    s8 field_56;
} Obj800E4EAC;

void func_800E4EAC(Obj800E4EAC *obj) {
    s8 rate = obj->field_34;
    s32 scale;

    if (rate & 0x80) {
        obj->field_56 = rate & 0x7F;
        return;
    }
    scale = (((rate >> 3) & 7) + 1) * 10;
    obj->field_56 = (s8)(obj->field_56 * scale / ((rate & 7) + 1) + 5) / 10;
}
