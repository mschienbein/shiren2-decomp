#include "common.h"

/* g++ 2.x vtable entry: this-adjust delta, index, function pointer. */
typedef struct { short delta; short index; void (*fn)(void *, s32); } VEntry;

typedef unsigned char u8;
typedef struct { s32 x0; s32 x4; VEntry *vt8; } Obj;
extern u8 D_80156A7B;
s32 func_80049CB4(s32 id, ...);
void func_80115E18(void *obj, void *position);
void func_800A00C4(void *origin, u8 percentage, void *attacker, s32 kind);
/* Trap slot +0x44 supplies direction and target-unit pointers, unused here. */
s32 func_801253AC(void *p, void *b, void *c, void *d, void *unused4, void *unused5, Obj *o) {
    func_80049CB4(0xF5, d);
    func_80115E18(p, c);
    if (o != 0) func_80049CB4(0xCD, o, d);
    func_800A00C4(d, D_80156A7B, b, 8);
    if (o == 0) return 1;
    o->vt8[1].fn((char *)o + o->vt8[1].delta, 3);
    return 0;
}
