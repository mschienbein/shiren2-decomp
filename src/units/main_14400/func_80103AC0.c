#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad_0[0x24]; const void *vtable_24; } Obj800EFC70;
extern void *func_800A38FC(s32 size);
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern const u8 D_8015BA78[];
void *func_80103AC0(u8 kind, Obj800EFC70 *p) { if (p==0) p=func_800A38FC(0xA0); func_800EFC70(p,0x43,kind); p->vtable_24=D_8015BA78; return p; }
