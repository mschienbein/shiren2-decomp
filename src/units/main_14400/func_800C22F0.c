#include "common.h"
typedef struct { unsigned char field0; s32 field4; void *field8; s32 fieldC; } Object;
extern unsigned char D_80143392;
extern s32 D_80153B40, D_8014A750;
extern void func_80042E90(unsigned char *);
Object *func_800C22F0(Object *p, unsigned char value) {
    p->field8 = &D_80153B40;
    p->field0 = value;
    p->field4 = 0;
    p->field8 = &D_8014A750;
    func_80042E90(&p->field0);
    p->fieldC = -1;
    D_80143392 = 0;
    return p;
}
