#include "common.h"
typedef unsigned char u8;
/* Collection capacity slot +0xC returns a full-width signed count. */
typedef struct { unsigned char pad0[8]; short adjust_8; short padA; s32 (*call_C)(void *); } VTable;
typedef struct { void *pool_0; VTable *field_4; } Object;
void func_800CE5B4(Object *self) {
    VTable *table = self->field_4;
    table->call_C((unsigned char *)self + table->adjust_8);
}
