#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef float f32;

typedef struct {
    u8 pad0[0x34];
    s32 field_34;
    s32 field_38;
    u8 pad3C[0x70 - 0x3C];
    f32 field_70;
    u8 pad74[0xA2 - 0x74];
    u16 field_A2;
    u16 field_A4;
    u8 padA6[0xBC - 0xA6];
    u8 field_BC;
    u8 padBD[0xDB - 0xBD];
    u8 depth;
    u8 padDC[0xE0 - 0xDC];
    u8 *stackPtr[4];
    s32 stack38[4];
    s32 stack34[4];
    u16 stackA2[4];
    u16 stackA4[4];
    u8 stackOp[4];
    u8 stackBC[4];
    f32 stack70[4];
} Obj801296E4;

u8 *func_801296E4(Obj801296E4 *obj, u8 *ptr) {
    u8 depth = obj->depth;

    obj->stackOp[depth] = *ptr++;
    obj->stackPtr[depth] = ptr;
    obj->stack38[depth] = obj->field_38;
    obj->stack34[depth] = obj->field_34;
    obj->stackBC[depth] = obj->field_BC;
    obj->stack70[depth] = obj->field_70;
    obj->stackA2[depth] = obj->field_A2;
    obj->stackA4[depth] = obj->field_A4;
    obj->depth++;
    return ptr;
}
