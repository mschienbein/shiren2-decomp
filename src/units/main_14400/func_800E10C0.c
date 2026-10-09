#include "common.h"
typedef struct { unsigned char pad0[0x40]; unsigned short field_40; } Unit800E5248;
void func_800E10C0(Unit800E5248 *self) { self->field_40 &= 0xFFF0; }
