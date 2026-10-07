#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 field0;
    u8 field1;
    u8 pad2[6];
    u8 block8[8];
    u8 block10[8];
} Obj800DB354;

extern void func_800DA9DC(u8 *out, u8 *src);

s32 func_800DB354(Obj800DB354 *obj, u8 *out) {
    *out++ = obj->field1;
    func_800DA9DC(out++, obj->block8);
    func_800DA9DC(out, obj->block10);
    return 3;
}
