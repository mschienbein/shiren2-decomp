#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef short s16;
typedef signed char s8;

typedef struct Obj_8010EAF4 Obj_8010EAF4;

typedef struct {
    u8 pad0[0x40];
    s16 this_offset;
    u8 pad42[2];
    u8 (*get_stat)(void *self, s32 stat);
} VTable_8010EAF4;

struct Obj_8010EAF4 {
    u8 pad0[0x8];
    VTable_8010EAF4 *vtable;
    s8 field_C;
};

extern s32 func_8010B9F4(Obj_8010EAF4 *obj);

u32 func_8010EAF4(Obj_8010EAF4 *obj) {
    s32 base = obj->field_C;
    s16 bonus = func_8010B9F4(obj);
    u8 plus = obj->vtable->get_stat((u8 *)obj + obj->vtable->this_offset, 0xE);
    u8 minus = obj->vtable->get_stat((u8 *)obj + obj->vtable->this_offset, 9);
    s32 total = base + bonus + plus - minus;

    if (total < 0) {
        return 0;
    }
    return 0xFFFF < total ? 0xFFFF : total;
}
