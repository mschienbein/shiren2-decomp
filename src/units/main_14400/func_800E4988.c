#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x8]; u8 dir; } Obj800E4988;
s32 func_800A99D0(void);
s32 func_800E47FC(Obj800E4988 *obj);
s32 func_80049CB4(s32 msg, ...);
void func_800E4988(Obj800E4988 *obj) {
    u8 saved;
    s32 dir;
    u8 tmp[4];
    s32 busy = func_800A99D0() != 1;
    if (busy) {
        saved = obj->dir;
        dir = func_800E47FC(obj);
        if (dir != -1) {
            tmp[0] = dir & 7;
            obj->dir = tmp[0];
            func_80049CB4(0x8B, obj);
        }
        obj->dir = saved;
    }
}
