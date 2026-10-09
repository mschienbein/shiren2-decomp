#include "common.h"

typedef unsigned char u8;
typedef struct Pos { s32 x, y; } Pos;
typedef struct VTable {
    u8 pad_00[0x38];
    short this_delta_38;
    short slot_3A;
    s32 (*place_3C)(void *, Pos *, s32);
} VTable;
typedef struct Obj {
    u8 pad_00[0x24];
    VTable *vtable_24;
} Obj;
extern s32 func_800A4360(void *object, void *position);

s32 func_800A4314(Obj *object, Pos *position)
{
    s32 kind = func_800A4360(object, position);
    return object->vtable_24->place_3C((u8 *)object + object->vtable_24->this_delta_38,
                                      position, kind);
}
