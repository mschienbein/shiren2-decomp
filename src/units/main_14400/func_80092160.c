#include "common.h"
typedef unsigned char u8;
typedef struct { short delta; short index; void *fn; } Pair;
typedef struct { char pad0[0x10]; char v10[0x10]; char *v20; u8 name[5]; char pad29[3]; char v2C[0x10]; char v3C[0x10]; } S;
extern const Pair D_80151470;
extern const Pair D_80151478;
extern char D_8014AAF4[];
extern char D_8014AB04[];
extern void func_80048728(void *);
extern void func_800922F4(void *, S *, Pair);
extern void func_800486A4(void *, void *, void *);
void func_80092160(S *p, char *text, u8 *name) {
    s32 i;
    func_80048728(p);
    func_80048728(p->v10);
    p->v20 = text;
    func_800922F4(p->v2C, p, D_80151470);
    func_800486A4(p, D_8014AAF4, p->v2C);
    if (name != 0) {
        for (i = 0; i < 5; i++) p->name[i] = name[i];
        func_800922F4(p->v3C, p, D_80151478);
        func_800486A4(p->v10, D_8014AB04, p->v3C);
    }
}
