#include "common.h"
typedef struct { unsigned char pad0[0x24]; void *vtable24; } Object;
extern unsigned char D_8015B748[];
extern void *func_800A38FC(s32 size);
extern void *func_800EFC70(void *object, s32 kind, unsigned char value);
Object *func_80101C90(unsigned char value, Object *object) {
    if (!object) object = func_800A38FC(0xA0);
    func_800EFC70(object, 0x3F, value);
    object->vtable24 = D_8015B748;
    return object;
}
