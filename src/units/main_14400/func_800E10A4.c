#include "common.h"
typedef struct { unsigned char pad0[0x40]; unsigned short field_40; } Object;
/* The sole original caller (func_800E11C0, 0x800E16F4) passes its int kind un-narrowed:
 * full-width parameter, narrowed to a byte here (0x800E10A4). */
void func_800E10A4(Object *self, s32 value) {
    self->field_40 = (self->field_40 & 0xFFF0) | ((unsigned char)value - 10);
}
