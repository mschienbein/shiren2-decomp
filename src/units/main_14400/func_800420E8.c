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
/* The caller passes a signed slot index; VRAM 0x80042108 (ROM 0x15148) narrows it here. */
void func_800420E8(s32 id) {
    Obj *obj;
    u8 byte_id;

    if (func_800610A8() != 0) {
        byte_id = id;
        obj = func_800A8CB0(byte_id);
        func_80049CB4(0x87, obj);
        func_80049CB4(2);
        if (obj != 0) {
            ((void (*)(void *, s32))obj->vtable[1].func)((u8 *)obj + obj->vtable[1].delta, 3);
        }
    }
}
