#include "common.h"

typedef unsigned char u8;
typedef struct { short offset; short pad; void *fn; } VEntry;
typedef struct { s32 f0; VEntry *vtbl; } Sub;
typedef struct { u8 kind; u8 type; char pad2[6]; VEntry *vtbl; Sub sub; } Self;
typedef struct { char pad[0xA]; u8 fA; } Item;
typedef struct { char pad[0xD]; u8 fD; u8 fE; } Info;
extern u8 D_80147F50[];
Item *func_801217DC(Self*);
s32 func_800E0F40(Item*);
s32 func_800D7D84(u8 kind, u8 level);
s32 func_800CDD68(Self *self){
    s32 value;
    Item *item;
    Info *info;
    Sub *sub;
    s32 kind;
    value = D_80147F50[self->kind] * 10000;
    if (self->type != 0xAC) return value;
    if (((s32 (*)(void*, s32))self->vtbl[3].fn)((char*)self + self->vtbl[3].offset, 0x1C)) {
        value += 8000;
        item = func_801217DC(self);
        if (item == 0) return value;
        kind = item->fA;
        value += func_800D7D84((u8)kind, (u8)func_800E0F40(item));
    } else if (((s32 (*)(void*, s32))self->vtbl[3].fn)((char*)self + self->vtbl[3].offset, 0x1B)) {
        sub = &self->sub;
        value += 7000;
        info = ((Info *(*)(void*, u32))sub->vtbl[7].fn)((char*)sub + sub->vtbl[7].offset, 0);
        if (info == 0) return value;
        value += func_800D7D84(info->fD, info->fE);
    } else {
        value += 9000;
    }
    return value;
}
