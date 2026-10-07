#include "common.h"

typedef unsigned char u8;

typedef struct {
    void *pool;
    void *vtable;
    u8 *entries;
    u8 count;
    u8 capacity;
    void *owner;
    short flags;
} ListMember800FA094;
typedef struct { u8 pad[0x24]; void *vtable; u8 pad2[0x7C]; ListMember800FA094 xA4; s32 xBC; } S;
extern u8 D_80159F60[];
void func_800CE6A0(ListMember800FA094 *, s32);
void func_800EFD28(S *, s32);
void func_800A3918(void *);
void func_800FA094(S *s, s32 flags) { s->vtable = D_80159F60; func_800CE6A0(&s->xA4, 2); func_800EFD28(s, 0); if (flags & 1) func_800A3918(s); }
