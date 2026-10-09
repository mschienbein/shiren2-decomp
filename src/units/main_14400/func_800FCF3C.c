#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct {
    s32 x;
    s32 y;
} Pos;
typedef struct {
    u8 pad[8];
    s16 delta;
    s16 padA;
    void (*destroy)(void *, s32);
} VTable;
typedef struct {
    u8 unk0;
    u8 kind;
    u8 pad2[6];
    VTable *vtable;
    u8 padC;
    u8 unkD;
} Item;
typedef struct {
    u8 pad[0xA0];
    void *unkA0;
} Actor;
void *func_800B4D80(void *);
u32 func_800B1C6C(void *);
void *func_800B4928(Pos *);
s32 func_800A99D0(void);
s32 func_80049CB4(s32, ...);
char *func_800AE674(void *);
void func_800498E4(s32, ...);
void func_800AD868(Pos *);
void *func_800FCE40(u8 variant, void *mem);
s32 func_800A3934(Actor *);
void func_800A58FC(Actor *, Pos *);
void func_800E2124(Actor *);
void func_800E20F0(Actor *);
void *func_800AACE0(s32);

s32 func_800FCF3C(Pos *pos, s32 mode)
{
    Item *item = func_800B4D80(pos);
    Actor *actor;
    s32 ok;

    ok = item != 0 && item->kind == 0xF2 && !(func_800B1C6C(pos) & 0x2000);
    if (!ok) {
        return 0;
    }
    if (func_800B4928(pos) != 0) {
        return 0;
    }
    if ((func_800A99D0() ^ 1) != 0) {
        item->unkD |= 0x80;
        func_80049CB4(0x120, pos);
        func_80049CB4(6);
        func_800498E4(0x226, func_800AE674(item));
        func_80049CB4(7);
        func_800AD868(pos);
        actor = func_800FCE40(item->unkD & 0x7F, 0);
        if ((func_800A3934(actor) ^ 1) != 0) {
            func_800A58FC(actor, pos);
            if (mode) {
                func_800E2124(actor);
            } else {
                func_800E20F0(actor);
            }
            if (item != 0) {
                item->vtable->destroy((u8 *)item + item->vtable->delta, 3);
            }
            actor->unkA0 = func_800AACE0(0);
            return 1;
        }
        if (item != 0) {
            item->vtable->destroy((u8 *)item + item->vtable->delta, 3);
        }
        func_80049CB4(6);
        func_800498E4(0x225);
        func_80049CB4(7);
    }
    return 0;
}
