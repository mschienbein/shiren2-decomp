#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad0[0x24]; VTable *vtable; } Object;
extern VTable D_8015BD78;
extern void *func_800A38FC(s32 size);
extern Object *func_800EFC70(Object *object, s32 kind, u8 value);
Object *func_80105370(u8 value, Object *storage) {
    if (storage == 0) storage = func_800A38FC(0xA0);
    func_800EFC70(storage, 0x47, value);
    storage->vtable = &D_8015BD78;
    return storage;
}
