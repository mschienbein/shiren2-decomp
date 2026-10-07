#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0xC]; void *xC; u8 x10[4]; } Obj;
s32 func_800ACA60(void *key, void *payload);
void func_800498E4(s32 message_id, ...);
s32 func_800A08D8(s32 mode, s32 key, s32 sel);
s32 func_800DCEBC(Obj *obj) {
    s32 failed = func_800ACA60(obj->xC, obj->x10) != 1;
    if (failed) {
        func_800498E4(0x297);
        func_800A08D8(1, -1, 0);
    }
    return 1;
}
