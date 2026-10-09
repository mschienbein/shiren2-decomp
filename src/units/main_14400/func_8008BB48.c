#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    u8 pad_0[4]; u16 state_4, index_6; u8 pad_8[6]; u16 field_E;
    u8 pad_10[4]; s32 field_14; u8 pad_18[0xC];
    s32 field_24, field_28, field_2C; u8 pad_30[0x2C];
    s32 field_5C, field_60, field_64, field_68, field_6C;
} Object;
extern s32 func_80084C60(s32 index, void (*callback)(void *), u32 tag);
extern void func_8007D264(s32, s32, s32, s32, s32, s32, s32);
void func_8008BB48(void *arg) {
    Object *p = arg;
    if (func_80084C60(p->index_6, func_8008BB48, p->field_14) < 0) {
        func_8007D264(p->field_24, p->field_28, p->field_5C, p->field_60,
                     p->field_68, p->field_6C, p->field_2C);
        p->field_E = 1;
        p->state_4 = 4;
    }
}
