#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad0[0xC]; u8 field_C; u8 field_D; } Obj800CE6F8;
/* Item-family slot +0x1C (root table D_80154300): void (self, s32 value).
 * The capacity limit is compared unsigned (sltu), as in the original. */
void func_800CE6F8(Obj800CE6F8 *obj, s32 value) {
    if (obj->field_C < (u32)value) value = obj->field_C;
    obj->field_D = value;
}
