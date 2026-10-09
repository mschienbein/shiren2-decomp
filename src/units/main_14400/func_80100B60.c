#include "common.h"
typedef unsigned char u8;
typedef struct VTable VTable;
typedef struct Obj800EFC70 { u8 pad_00[0x24]; const VTable *vtable_24; u8 pad_28[0x72]; unsigned short flags_9A; } Obj800EFC70;
extern const VTable D_8015B380;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern void func_800E4D88(Obj800EFC70 *obj, s32 value);
extern void func_800E4D90(Obj800EFC70 *a, s32 b);
Obj800EFC70 *func_80100B60(Obj800EFC70 *self, u8 value) {
    func_800EFC70(self, 57, value);
    self->vtable_24 = &D_8015B380;
    func_800E4D88(self, 3);
    func_800E4D90(self, 2);
    self->flags_9A |= 7;
    return self;
}
