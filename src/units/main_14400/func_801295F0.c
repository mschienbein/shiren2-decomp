#include "common.h"

typedef unsigned char u8;
typedef float f32;

typedef struct Obj801295F0 {
    u8 pad_00[0x30];
    f32 value_30;
    u8 pad_34[0x90 - 0x34];
    f32 offset_90;
} Obj801295F0;

/* Script opcode (table D_801487D0): signed byte operand / 100 replaces the offset applied to value_30. */
u8 *func_801295F0(Obj801295F0 *obj, u8 *p) {
    s32 raw = *p++;
    f32 offset;

    if (raw & 0x80) {
        raw |= ~0xFF;
    }
    offset = (f32)raw / 100.0;
    obj->value_30 = obj->value_30 - obj->offset_90 + offset;
    obj->offset_90 = offset;
    return p;
}
