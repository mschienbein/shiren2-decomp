#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { u8 v; } Byte;
/* Slot 13 is func_800E0E88; its unsigned result is narrowed by the caller. */
typedef struct { s16 delta; s16 index; u32 (*func)(void *); } VEntry;
typedef struct {
    u8 pad0[8];
    Byte unk8;
    u8 pad9[0x24 - 9];
    VEntry *vtable;
    u8 pad28[0x58 - 0x28];
    void *unk58;
    u8 pad5C[0x89 - 0x5C];
    u8 unk89;
} S;
s32 func_800E20CC(void *obj);
s32 func_800E0F40(void *obj);
void *func_800B371C(void *src, Byte b, s32 n, s32 k);
s32 func_800A5758(void *a, void *b, short c, unsigned short d);
/* Monster slot +0xB4 act(self, target): the target supplied by the call contract is unused here;
 * the slot result is func_800A5758's, as in the original (v0 untouched before the return). */
s32 func_8010725C(S *self, void *target_unused) {
    void *value;
    if (func_800E20CC(self)) {
        if ((u8)func_800E0F40(self) == 3) {
            value = func_800B371C(self, self->unk8, self->unk89, 0xFF);
        } else {
            value = 0;
        }
    } else {
        value = self->unk58;
    }
    return func_800A5758(self, value, (s16)self->vtable[13].func((u8 *)self + self->vtable[13].delta), 0);
}
