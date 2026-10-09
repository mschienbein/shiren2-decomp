#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct Obj800EFC70 { u8 pad_00[0x24]; const VTable *vtable_24; } Obj800EFC70;
extern const VTable D_8015AAC0;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
Obj800EFC70 *func_800FE8A8(Obj800EFC70 *self, u8 value) {
    func_800EFC70(self, 46, value);
    self->vtable_24 = &D_8015AAC0;
    return self;
}
