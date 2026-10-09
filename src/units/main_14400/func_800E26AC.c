#include "common.h"
/* Slot +0x94 targets func_800E115C / derived dispatchers; +0x90 is zero in their tables. */
typedef struct { unsigned char field_00[0x90]; short field_90; s32 (*field_94)(void *, s32, s32, unsigned char, s32); } Methods;
typedef struct { unsigned char field_00[0x24]; Methods *field_24; } Object;
void func_800E26AC(Object *self) {
    Methods *methods = self->field_24;
    methods->field_94((unsigned char *)self + methods->field_90, 1, 0x14, 0, 0);
}
