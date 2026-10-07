#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0xC]; u8 flags; } Obj80112EE4;
void *func_800AC5F4(s32 size, Obj80112EE4 *obj);
void *func_8011AF20(void *obj);
s32 func_80112EE4(Obj80112EE4 *obj) {
    u32 has_child = (obj->flags >> 1) & 1;
    if (has_child != 0) {
        func_8011AF20(func_800AC5F4(0x10, obj));
        return 1;
    }
    return 0;
}
