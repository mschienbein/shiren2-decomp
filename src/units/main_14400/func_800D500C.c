#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef s32 (*Predicate)(void *);
/* +0x10 is populated by func_800A1308 and released by func_800A12BC. */
typedef struct { void *from; void *to; u16 text; Predicate predicate; void *temporary10; } Transfer;
typedef struct { s16 delta, index; void *(*get)(void *); } Accessor;
typedef struct { u8 pad00[0x98]; Accessor list98; } Vtable;
typedef struct { u8 pad00[0x24]; Vtable *vtable; } Manager;
typedef struct { u8 pad00[8]; s8 mode08; u8 pad09[3]; void *list0C; } Object;
extern Manager *D_801476B8;
extern s32 func_8009A2E8(void *);
/* The original stores two object pointers and a callback, not integer carriers. */
extern void func_800A12A4(Transfer *, void *, void *, u16, Predicate);
extern s32 func_800A12BC(Transfer *, u16);
s32 func_800D500C(Object *object, s32 mode)
{
    Transfer transfer;
    Manager *manager = D_801476B8;
    void *list = manager->vtable->list98.get((u8 *)manager + manager->vtable->list98.delta);
    Predicate predicate = object->mode08 == 1 ? func_8009A2E8 : 0;
    s32 result = 3;
    switch (mode) {
    case 0:
        func_800A12A4(&transfer, list, object->list0C, 0x1ED, predicate);
        result = func_800A12BC(&transfer, 0x1EF);
        break;
    case 1:
        func_800A12A4(&transfer, object->list0C, list, 0x1EE, 0);
        result = func_800A12BC(&transfer, 0);
        break;
    }
    return result;
}
