#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad; unsigned char x1; char pad2[0xA]; unsigned short xC; } Ent;
extern void *D_801476B8;
extern s32 D_80147678;
Ent *func_800B4D80(void *p);
void func_801F212C(u16 id, u8 flag);
s32 func_800DFCD0(void *self /* receiver: unused; supplied by the vtable +0x14 call */) {
    Ent *e = func_800B4D80(D_801476B8);
    unsigned short id;
    if (e == 0) return 1;
    switch (e->x1) {
    case 0xCE:
        id = e->xC;
        if (id == 0) {
            D_80147678 = 6;
            break;
        }
        func_801F212C(id, 0);
        return 1;
    case 0xCF:
        D_80147678 = 8;
        break;
    default:
        return 1;
    }
    return 1;
}
