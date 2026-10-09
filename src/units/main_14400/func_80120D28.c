#include "common.h"
typedef struct { unsigned char pad0[2]; unsigned char field_2; unsigned char pad3[0x25]; unsigned char field_28; } Obj;
s32 func_80120D28(Obj *obj, s32 kind) {
    if (kind == 0x1D) return obj->field_28 != 0;
    if (kind == 0xE) {
        unsigned char flag = obj->field_2 & 4;
        return flag == 0;
    }
    return kind == 0x11 || kind == 0xB || kind == 0x16 || kind == 2;
}
