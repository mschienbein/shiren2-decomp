#include "common.h"
typedef struct { unsigned char pad_00[8]; const void *field_08; } Object;
extern const unsigned char D_8015DB38[];
extern void *func_800AC0C0(void *self, s32 a, s32 b);
void *func_80117230(Object *self, s32 kind) { func_800AC0C0(self, 0x13, kind); self->field_08 = D_8015DB38; return self; }
