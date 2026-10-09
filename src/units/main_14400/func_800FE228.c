#include "common.h"
typedef unsigned char u8;
struct VTable;
typedef struct { unsigned char pad0[0x24]; const struct VTable *field_24; } Obj800EFC70;
extern const struct VTable D_8015A940;
Obj800EFC70 *func_800EFC70(Obj800EFC70 *self, s32 kind, u8 value);
Obj800EFC70 *func_800FE228(Obj800EFC70 *self, u8 value) {
    func_800EFC70(self, 0x2C, value);
    self->field_24 = &D_8015A940;
    return self;
}
