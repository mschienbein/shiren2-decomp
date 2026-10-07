#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s16 delta; s16 pad; void *func; } VEntry;
typedef struct { s32 a; s32 b; } Pair;
typedef struct { s32 field_0; s32 field_4; s32 field_8; u8 padC[0xC]; VEntry *vtable; } Obj;
void func_800CA0A8(Obj *obj, s32 pos);
void func_800CA408(Obj *obj, Pair *src) {
    obj->field_4 = src->a;
    obj->field_8 = src->b;
    func_800CA0A8(obj, 0);
    ((void (*)(void *, s32, void *))obj->vtable[3].func)((u8 *)obj + obj->vtable[3].delta, 4, &obj->field_4);
    func_800CA0A8(obj, 4);
    ((void (*)(void *, s32, void *))obj->vtable[3].func)((u8 *)obj + obj->vtable[3].delta, 4, &obj->field_8);
    ((void (*)(void *))obj->vtable[1].func)((u8 *)obj + obj->vtable[1].delta);
    func_800CA0A8(obj, obj->field_4);
}
