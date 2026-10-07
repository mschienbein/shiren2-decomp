#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0; u8 kind; u8 pad2; u8 mode; } Obj800AD7E0;
u32 func_800B1C6C(void *pos);
void func_800B4C64(void *pos, Obj800AD7E0 *obj);
s32 func_80049CB4(s32 msg, ...);
void func_800AD7E0(Obj800AD7E0 *obj, void *pos, s32 notify) {
    u32 mode = func_800B1C6C(pos);
    obj->mode = (mode & 0x2000) ? 1 : 2;
    func_800B4C64(pos, obj);
    if (notify != 0) {
        func_80049CB4(0xD7, pos);
        if (obj->kind == 0xD5) {
            func_80049CB4(0xE2, pos);
        }
    }
}
