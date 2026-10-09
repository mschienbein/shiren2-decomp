#include "common.h"
typedef unsigned char u8;
typedef struct { unsigned char pad0[0x90]; short adjust_90; short pad92; s32 (*call_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { unsigned char pad0[0x24]; VTable *field_24; } Object;
s32 func_800E2934(Object *self) {
    VTable *table = self->field_24;
    return table->call_94((unsigned char *)self + table->adjust_90, 1, 15, 0, 0);
}
