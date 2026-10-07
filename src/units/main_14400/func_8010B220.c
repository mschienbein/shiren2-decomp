#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    void *pool;
    void *vtable;
    u8 *entries;
    u8 count;
    u8 capacity;
    void *owner;
    u16 flags;
} ListMember8010B220;
extern char D_8015CB28[], D_8015CB48[];
extern void *func_800EE3C0(void *, s32, u8);
extern void *func_800CEC90(ListMember8010B220 *, void *, void *, u8, u16);
extern s32 func_800A3934(void *);
extern void func_8010B380(void *, u8);
typedef struct { char pad[0x24]; void *vt2; char pad2[0x8C]; void *vt; char pad3[8]; char xC0[0xC]; ListMember8010B220 xCC; } Obj;
Obj *func_8010B220(Obj *self, u8 k){ func_800EE3C0(self, 0x1C, k); self->vt = D_8015CB28; self->vt2 = D_8015CB48; func_800CEC90(&self->xCC, self, self->xC0, 10, 0x1FF); if (func_800A3934(self) == 0) func_8010B380(self, k); return self; }
