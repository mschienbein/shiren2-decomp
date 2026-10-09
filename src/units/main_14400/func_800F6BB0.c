#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 pad_00[0x1C]; u16 field_1C; u8 pad_1E[6]; const void *field_24; } Object;
extern const u8 D_801496E0[];
extern void *func_800F3CF0(void *obj, s32 kind, u8 arg);
void *func_800F6BB0(Object *self, u8 arg) { func_800F3CF0(self, 0x58, arg); self->field_24 = D_801496E0; self->field_1C |= 0x80; return self; }
