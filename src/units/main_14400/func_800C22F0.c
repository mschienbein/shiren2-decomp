#include "common.h"
typedef struct { unsigned char field0; s32 field4; const void *field8; s32 fieldC; } Object;
extern unsigned char D_80143392;
extern const s32 D_80153B40[]; /* Address-only view of the 0x44-byte vtable. */
extern const unsigned char D_8014A750[68];
extern void func_80042E90(unsigned char *);
/* first/second: the original constructor call in func_800AA9FC (0x800AAAA4..0x800AAAAC)
 * supplies a2 = a3 = -1. This constructor never reads them (0x800C22F0..0x800C2330). */
Object *func_800C22F0(Object *p, unsigned char value, signed char first, signed char second) {
    p->field8 = &D_80153B40;
    p->field0 = value;
    p->field4 = 0;
    p->field8 = D_8014A750;
    func_80042E90(&p->field0);
    p->fieldC = -1;
    D_80143392 = 0;
    return p;
}
