#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad0[0x24]; VTable *field24; } Object;
extern VTable D_8015A940;
extern void *func_800A38FC(s32 size);
extern Object *func_800EFC70(Object *obj, s32 arg1, u8 arg2);

Object *func_800FDB60(u8 kind, Object *self)
{
    if (self == 0) {
        self = func_800A38FC(0xA0);
    }
    func_800EFC70(self, 0x2C, kind);
    self->field24 = &D_8015A940;
    return self;
}
