#include "common.h"
typedef unsigned char u8;
struct VTable;
typedef struct { u8 pad0[0x24]; const struct VTable *field_24; u8 pad28[0x72]; unsigned short field_9A; } Obj;
extern const struct VTable D_8015B1E8;
Obj *func_800EFC70(Obj *self, s32 kind, u8 value);
void func_800E4D88(Obj *self, s32 value);
void func_800E4D90(Obj *self, s32 value);
Obj *func_801002E0(Obj *self, u8 value) {
    func_800EFC70(self, 0x37, value);
    self->field_24 = &D_8015B1E8;
    func_800E4D88(self, 3);
    func_800E4D90(self, 2);
    self->field_9A |= 1;
    return self;
}
