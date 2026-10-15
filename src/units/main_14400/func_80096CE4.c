#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef struct { u8 a; u8 b; u8 c; u8 d; } Bytes4;
typedef struct { u8 kind; u8 pad[0xE]; s8 fF; } Target;
/* Menu descriptor (func_8009D910 copies it whole; func_8009543C reads +0x00 as the s32 count).
   Only the count's low byte is used here, read as lbu +0x03. */
typedef struct { s32 count; } Src;
typedef struct { u8 pad[0x74]; Target *target; } Self;
u8 *func_8006A810(void *, s32, s32);
s32 func_800ACEB4(Target *);
void func_8009D910(Self *, Src *, Bytes4 *);
void func_80096CE4(Self *self, Target *target, Src *src) {
    Bytes4 out;
    Bytes4 tmp;
    u16 kind;
    s32 bonus;
    self->target = target;
    func_8006A810(&tmp, 0, 4);
    tmp.a = (u8)src->count;
    tmp.b = 1;
    tmp.d = (u8)src->count;
    out = tmp;
    kind = self->target->kind;
    if (func_800ACEB4(self->target) == 2 && (kind == 3 || kind == 4)) {
        bonus = self->target->fF;
        if (bonus > 0) {
            out.d = bonus + 2;
        }
    }
    func_8009D910(self, src, &out);
}
