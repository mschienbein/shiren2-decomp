#include "common.h"
typedef unsigned char u8;
/* Second collection/item link at +0x10; func_800DA9DC reads its item pointer at +4. */
typedef struct { void *collection; void *item; } Link;
typedef struct { u8 field_00; u8 field_01; u8 pad_02[14]; Link field_10; } Object;
extern void func_800DA9DC(u8 *out, Link *link);
s32 func_800DB708(Object *object, u8 *out) {
    *out++ = object->field_01;
    func_800DA9DC(out, &object->field_10);
    return 2;
}
