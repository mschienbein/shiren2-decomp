#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    s32 unk0;
    s32 unk4;
} Pair800A59A4;

void *func_800B4928(Pair800A59A4 *pos);
void *func_800B49B8(Pair800A59A4 *pos);
s32 func_80049CB4(s32 id, ...);

void func_800A59A4(Pair800A59A4 *self) {
    Pair800A59A4 pos;
    Pair800A59A4 *p = &pos;

    pos.unk0 = self->unk0;
    p->unk4 = self->unk4;
    if (func_800B4928(p) == self) {
        func_800B49B8(p);
        func_80049CB4(0xD8, p);
    }
}
