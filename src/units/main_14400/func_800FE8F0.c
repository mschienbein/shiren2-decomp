#include "common.h"

typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct { u8 pad00[0x24]; const VTable *vtable24; u8 pad28[0x78]; } Obj800EFC70;
extern const VTable D_8015AB80;
extern void *func_800A38FC(s32 size);
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);

Obj800EFC70 *func_800FE8F0(u8 kind, Obj800EFC70 *self) {
    if (self == 0) self = func_800A38FC(0xA0);
    func_800EFC70(self, 0x2F, kind);
    self->vtable24 = &D_8015AB80;
    return self;
}
