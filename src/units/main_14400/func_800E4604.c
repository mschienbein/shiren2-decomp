#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct {
    s32 x;
    s32 y;
} Pos;
typedef struct { u8 value; } Dir;
typedef struct {
    u8 pad[8];
    s16 delta8;
    s16 padA;
    void (*destroy)(void *, s32);
    u8 pad10[0x28];
    s16 delta38;
    s16 pad3A;
    s32 (*canMove)(void *, Pos *, s32);
} VTable;
typedef struct {
    u8 pad[0x24];
    VTable *vtable;
    u8 pad28[0x2C];
    u8 flags;
} Actor;
typedef struct {
    s32 x;
    s32 y;
    u8 dir;
    u8 unk9;
    u8 padA[0x4A];
    u8 flags;
} Source;
extern s32 D_80148308[];
void func_800A2F80(Dir *, s32);
void *func_800A2594(void *, void *, Dir);
void func_800A58FC(Actor *, Pos *);
void func_800A665C(Actor *, Dir *);
s32 func_80049CB4(s32, ...);

s32 func_800E4604(Source *src, Actor *actor)
{
    Pos pos;
    Dir dir;
    s32 i;

    if (!(src->flags & 8)) {
        pos.x = src->x;
        pos.y = src->y;
        dir.value = src->dir;
        i = 0;
        for (;;) {
            Pos next;

            if (i >= 8) {
                break;
            }
            func_800A2F80(&dir, D_80148308[i]);
            func_800A2594(&next, &pos, dir);
            if (actor->vtable->canMove((u8 *)actor + actor->vtable->delta38, &next, src->unk9 & 0xF)) {
                func_800A58FC(actor, &next);
                func_800A665C(actor, &dir);
                func_80049CB4(0x78, actor, &pos);
                actor->flags |= 8;
                return 1;
            }
            i++;
        }
        if (actor != 0) {
            actor->vtable->destroy((u8 *)actor + actor->vtable->delta8, 3);
        }
    }
    return 0;
}
