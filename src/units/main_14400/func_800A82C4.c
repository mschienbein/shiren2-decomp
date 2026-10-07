#include "common.h"

typedef struct { unsigned char pad0[0x1C]; unsigned short unk1C; } Obj;

s32 func_800A82C4(Obj *obj) {
    unsigned short flags = obj->unk1C & 2;
    return flags != 0;
}
