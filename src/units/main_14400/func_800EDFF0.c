#include "common.h"
typedef unsigned char u8;
/* Player table D_80159008+0x64 targets void func_800ECC74(void *). */
typedef struct { u8 pad_00[0x60]; short adjust_60; short pad_62; void (*refresh_64)(void *); } VTable;
typedef struct { u8 pad_00[0x24]; VTable *vtable_24; } Object;
extern u32 D_8013960C;
/* EAFB8 -> EAF7C -> E5E4C consumes both receiver and notification flag. */
extern s32 func_800EAFB8(void *self, s32 notify);
extern void func_800EBF34(void *self);
extern void func_800EBCD0(void *self);
void func_800EDFF0(Object *self, s32 notify) {
    func_800EAFB8(self, notify);
    D_8013960C <<= 1;
    func_800EBF34(self);
    func_800EBCD0(self);
    D_8013960C >>= 1;
    self->vtable_24->refresh_64((u8 *)self + self->vtable_24->adjust_60);
}
