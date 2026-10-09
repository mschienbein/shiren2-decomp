#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* Contained collection/item link at +8; func_800DA9DC reads its item pointer at +4. */
typedef struct { void *collection; void *item; } Link;
typedef struct { union { u16 value; u8 bytes[2]; } field_00; u8 pad_02[6]; Link field_08; } Object;
extern const u8 D_80151A3C[64];
extern void func_800DA9DC(u8 *out, Link *link);
s32 func_800DAA14(Object *self, u8 *out) { out[0] = self->field_00.bytes[1]; func_800DA9DC(out + 1, &self->field_08); return D_80151A3C[self->field_00.value]; }
