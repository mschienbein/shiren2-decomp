#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct ShirenDirection { signed char value; } ShirenDirection;
typedef struct { u8 value; } Dir;
typedef struct { s32 x; s32 y; } Point;
typedef struct { u8 pad_0[0x20]; u32 field_20; } Owner;
typedef struct { u8 pad_0[0x1C]; const void *field_1C; Owner *field_20; u32 field_24; u16 field_28; u16 pad_2A; Point field_2C; s32 field_34; u16 field_38; u8 field_3A; u8 pad_3B; void *field_3C; void *field_40; } Object;
extern const u8 D_80153FA8[];
extern void *func_800A2594(Point *, void *, Dir);
extern void *func_800C2CA0(u8 *, void *, void *, s32, s32);
extern void func_80136908(u32 *);
void *func_800C4360(Object *object, void *owner, u16 value, s32 command, void *position, ShirenDirection direction, s32 limit, u16 flags, u8 mode) {
    Point next; Dir dir;
    dir.value = direction.value;
    func_800A2594(&next, position, dir);
    func_800C2CA0((u8 *)object, &next, &direction.value, limit, 2);
    object->field_1C = D_80153FA8;
    func_80136908(&object->field_24);
    object->field_20 = owner;
    if (owner) object->field_24 = ((Owner *)owner)->field_20;
    object->field_28 = value;
    if (!command) object->field_34 = 0x100; else object->field_34 = command;
    object->field_2C = *(Point *)position;
    object->field_38 = flags; object->field_3A = mode; object->field_3C = 0; object->field_40 = 0;
    return object;
}
