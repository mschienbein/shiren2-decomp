#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    u8 pad_00[0x24]; float field_24; u8 pad_28[0xC]; void *field_34; void *field_38;
    u8 pad_3C[0x30]; float field_6C; float field_70; u8 pad_74[0x2E]; u16 field_A2; u16 field_A4;
    u8 pad_A6[0x16]; u8 field_BC; u8 pad_BD[0x1E]; u8 depth; u8 pad_DC[4];
    u8 *saved_pc[4]; void *saved_38[4]; void *saved_34[4]; u16 saved_A2[4]; u16 saved_A4[4];
    u8 remaining[4]; u8 saved_BC[4]; float saved_70[4];
} Object;

u8 *func_80129750(Object *object, u8 *pc) {
    s32 index = object->depth - 1;
    if (object->remaining[index] != 0xFF) {
        if (--object->remaining[index] == 0) {
            object->depth = index;
            index = -1;
        }
    }
    if (index >= 0) {
        pc = object->saved_pc[index];
        object->field_38 = object->saved_38[index];
        object->field_34 = object->saved_34[index];
        object->field_BC = object->saved_BC[index];
        object->field_70 = object->saved_70[index];
        object->field_A2 = object->saved_A2[index];
        object->field_A4 = object->saved_A4[index];
        object->field_24 = object->field_70 * object->field_6C;
    }
    return pc;
}
