#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct {
    u8 pad0[0x28];
    s16 this_offset;
    u8 pad2A[2];
    void (*set)(void *self, s32 size, void *data);
} VTable800EF868;
typedef struct { u8 pad0[0x18]; VTable800EF868 *vtable; } Obj800EF868;
extern u8 D_80159244[];
void func_800CA4E8(Obj800EF868 *obj, void *desc);
void *func_800EF72C(u8 id);
s32 func_800EF74C(u8 id);
void *func_800EF76C(u8 id);
s32 func_800EF78C(u8 id);
void func_800EF868(Obj800EF868 *obj) {
    s32 i;
    func_800CA4E8(obj, D_80159244);
    for (i = 0x18; i < 0x1D; i++) {
        obj->vtable->set((u8 *)obj + obj->vtable->this_offset, func_800EF74C(i), func_800EF72C(i));
        obj->vtable->set((u8 *)obj + obj->vtable->this_offset, func_800EF78C(i), func_800EF76C(i));
    }
}
