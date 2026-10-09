#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x4C]; const void *field_4C; u8 pad_50[0x10]; s32 field_60; void *field_64; u8 pad_68[0x10]; u8 field_78[0x20]; } Work;
extern u8 D_80152130[], D_80151EC8[], D_80152000[];
extern const unsigned char D_80151E38[144];
extern void *func_800A8CB0(s32);
extern Work *func_800953C0(Work *);
extern void func_8009702C(Work *, void *);
extern s32 func_800957C0(Work *, void *, s32, void *, s32);
void func_80042758(s32 handle) { Work work; Work *self = &work; void *target = func_800A8CB0((u8)handle); func_800953C0(self); self->field_4C = D_80152130; work.field_60 = -1; work.field_64 = D_80151EC8; self->field_4C = D_80152000; func_8009702C(self, target); func_800957C0(self, work.field_78, 1, 0, 0); self->field_4C = D_80151E38; }
