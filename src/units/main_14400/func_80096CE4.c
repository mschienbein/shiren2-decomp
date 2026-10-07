#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef struct { u8 a; u8 b; u8 c; u8 d; } Bytes4;
typedef struct { u8 kind; u8 pad[0xE]; s8 fF; } Target;
typedef struct { u8 pad[3]; u8 f3; } Src;
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
    tmp.a = src->f3;
    tmp.b = 1;
    tmp.d = src->f3;
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
