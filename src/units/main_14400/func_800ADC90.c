#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { s32 x; s32 y; } Position;
typedef struct { u8 pad00[8]; s16 delta08; s16 slot0A; void (*destroy0C)(void *, s32); } Vtable;
typedef struct { u8 pad00[8]; Vtable *vtable08; } Object;
extern s32 func_800ADD50(void *object, Position *position);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800AD8AC(void *object, Position *position);
extern void func_800D3650(void *object);

s32 func_800ADC90(Object *object, Position *position, void *origin)
{
    Position candidate;
    Position *next = &candidate;
    candidate.x = position->x;
    next->y = position->y;
    if (func_800ADD50(object, next)) {
        func_80049CB4(0x10C0, object, origin, next);
        return func_800AD8AC(object, next);
    }
    func_80049CB4(0x10C0, object, origin, position);
    func_800D3650(object);
    if (object != 0)
        object->vtable08->destroy0C((u8 *)object + object->vtable08->delta08, 3);
    return 0;
}
