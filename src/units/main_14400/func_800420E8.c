#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s16 delta; s16 pad; void *func; } VEntry;
typedef struct { u8 pad0[0x24]; VEntry *vtable; } Obj;
s32 func_800610A8(void);
void *func_800A8CB0(s32 cell);
s32 func_80049CB4(s32 id, ...);
void func_800420E8(u8 id) {
    Obj *obj;

    if (func_800610A8() != 0) {
        obj = func_800A8CB0(id);
        func_80049CB4(0x87, obj);
        func_80049CB4(2);
        if (obj != 0) {
            ((void (*)(void *, s32))obj->vtable[1].func)((u8 *)obj + obj->vtable[1].delta, 3);
        }
    }
}
