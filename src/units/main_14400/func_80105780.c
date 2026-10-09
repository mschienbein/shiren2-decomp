#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad00[0x24]; const VTable *vtable24; } Object;
extern void *func_800A38FC(s32 size);
extern void *func_800EFC70(void *object, s32 kind, u8 mode);
extern const VTable D_8015BE38;

Object *func_80105780(u8 kind, Object *object)
{
    if (object == 0)
        object = func_800A38FC(0xA0);
    func_800EFC70(object, 0x48, kind);
    object->vtable24 = &D_8015BE38;
    return object;
}
