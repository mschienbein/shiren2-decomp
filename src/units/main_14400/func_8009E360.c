#include "common.h"
/* Widget vtable slot 13 entry (+0x68 this-adjust, +0x6C method): value of a flattened
 * item index, 0x80000000 = none. */
typedef struct { short delta; short index; s32 (*fn)(void *self, s32 index); } ValueSlot;
typedef struct { char pad[0x68]; ValueSlot value; } VTable;
typedef struct { unsigned char unk0; } Info;
typedef struct { char pad[0x4C]; VTable *vtbl; char pad50[4]; Info *unk54; } Obj;
extern char D_80152D4C[];
s32 func_8010BC2C(Info *, unsigned char);
s32 func_800A2910(unsigned char, unsigned char, unsigned short *);
char *func_80048480(unsigned short);
s32 func_800327C0(char *, const char *, ...);
/* Widget vtable slot 11 (+0x58/+0x5C): writes the text of item `kind` into buf. */
void func_8009E360(Obj *o, s32 kind, char *buf) {
    ValueSlot *e;
    unsigned char r;
    unsigned short ids[2];
    char *a, *b;
    buf[0] = 0;
    e = &o->vtbl->value;
    if (e->fn((char *)o + e->delta, kind) == 0x80000000) return;
    r = (unsigned char)func_8010BC2C(o->unk54, kind);
    if (r == 0) return;
    func_800A2910(o->unk54->unk0, r, ids);
    a = func_80048480(ids[0]);
    b = func_80048480(ids[1]);
    func_800327C0(buf, D_80152D4C, a, b);
}
