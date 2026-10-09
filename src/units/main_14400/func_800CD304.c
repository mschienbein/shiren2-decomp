#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* List vtable slots (D_80154390/D_80154438/D_80154550): +0x24 func_800CE710 s32 count(self),
 * +0x4C func_800CE81C/func_800CEE58 void remove(self, s32 index). */
typedef struct { s16 delta; s16 pad; s32 (*func)(void *self); } CountSlot;
typedef struct { s16 delta; s16 pad; void (*func)(void *self, s32 index); } RemoveSlot;
typedef struct {
    u8 pad0[0x20];
    CountSlot count;
    u8 pad28[0x48 - 0x28];
    RemoveSlot remove;
} VTable;
typedef struct { void *pool; VTable *vtable; } Obj;
void func_800CD304(Obj *obj, u32 value) {
    if (value < obj->vtable->count.func((u8 *)obj + obj->vtable->count.delta)) {
        obj->vtable->remove.func((u8 *)obj + obj->vtable->remove.delta, (s32)value);
    }
}
