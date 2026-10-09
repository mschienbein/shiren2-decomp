#include "common.h"
typedef struct { s32 x, y; } Pos;
typedef struct { unsigned char pad0[8]; short adjust_8; short padA; void (*call_C)(void *, s32); } VTable;
typedef struct { unsigned char pad0[0x1E]; unsigned char flags; unsigned char pad1F[5]; VTable *field_24; } Object;
void *func_800B49B8(Pos *pos);
static inline s32 unprotected(Object *self) { return ((self->flags >> 2) & 1) ^ 1; }
void func_800B4A1C(Pos *pos) {
    Object *self = func_800B49B8(pos);
    if (self && unprotected(self)) {
        VTable *table = self->field_24;
        table->call_C((unsigned char *)self + table->adjust_8, 3);
    }
}
