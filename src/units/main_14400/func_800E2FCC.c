#include "common.h"
typedef unsigned char u8;
/* D_80158C98 and D_80159320 slot +0x94 target func_800E115C. */
typedef struct { u8 pad_00[0x90]; short adjust_90; short pad_92; s32 (*call_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { u8 pad_00[0x24]; VTable *vtable_24; } Object;
s32 func_800E2FCC(Object *self) { return self->vtable_24->call_94((u8 *)self + self->vtable_24->adjust_90, 1, 4, 0, 0); }
