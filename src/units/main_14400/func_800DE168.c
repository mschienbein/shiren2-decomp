#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s16 delta; s16 pad; void *func; } VEntry;
typedef struct { s32 field_0; VEntry *vtable; } Sub;
typedef struct { u8 field_0; u8 field_1; u8 pad2[0xAE]; Sub sub; } Obj;
void func_800DDB64(Obj *obj, u8 *buf, s32 count);
s32 func_800DE168(Obj *obj, u8 *buf) {
    Sub *sub = &obj->sub;
    s32 count = ((s32 (*)(void *))sub->vtable[4].func)((u8 *)sub + sub->vtable[4].delta);

    *buf = obj->field_1;
    buf++;
    *buf = count + 1;
    buf++;
    func_800DDB64(obj, buf, count);
    return count + 3;
}
