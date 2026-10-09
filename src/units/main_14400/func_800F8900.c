#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad00[0x24]; const VTable *vtable24; u8 pad28[0x58]; u8 field80; } Object;
extern void *func_800F3CF0(void *object, s32 kind, u8 arg);
extern s32 func_800A3934(void *object);
extern const VTable D_80159D50;

void *func_800F8900(Object *object, u8 kind)
{
    func_800F3CF0(object, 0x5D, kind);
    object->vtable24 = &D_80159D50;
    if (func_800A3934(object) == 0)
        object->field80 = 0;
    return object;
}
