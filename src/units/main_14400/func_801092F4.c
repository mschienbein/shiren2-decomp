#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { u8 pad00[0xA0]; s16 deltaA0; s16 slotA2; void *(*lookupA4)(void *, u8); } Vtable;
typedef struct { u8 pad00[0x24]; Vtable *vtable24; u8 pad28[0x50]; s32 field78; u8 pad7C[0x3C]; s16 fieldB8; u8 padBA[6]; s32 fieldC0; } Object;
extern void func_800E4D88(void *object, s32 value);
extern void func_800E4D90(void *object, s32 value);
extern void func_800E946C(void *object, s16 count);

void func_801092F4(Object *object, s32 kind)
{
    object->fieldC0 = 0;
    object->fieldB8 = 0x158;
    func_800E4D88(object, 4);
    func_800E4D90(object, 3);
    object->field78 = *(s32 *)object->vtable24->lookupA4((u8 *)object + object->vtable24->deltaA0, (u8)kind);
    func_800E946C(object, (u8)kind - 1);
}
