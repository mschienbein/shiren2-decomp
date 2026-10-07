#include "common.h"

typedef unsigned char u8;
typedef struct { s32 w[12]; } Block30;
typedef struct { void *handle0; s32 x4; s32 done8; } S;
u32 func_800D07D8(void *owner, s32 index);
Block30 *func_800AFD78(void *table, u8 id);
void func_800AFCD8(void *table, u8 id);
void func_800AFCA8(void *table, u8 id);
void func_800AE648(Block30 *b);
void func_800D0908(S *p, s32 arg, Block30 *src) {
    u8 id = (u8)func_800D07D8(p, arg);
    Block30 *dst = func_800AFD78(p->handle0, id);
    if (dst == 0) return;
    if (src != 0) {
        *dst = *src;
        func_800AFCD8(p->handle0, id);
        func_800AE648(dst);
    } else {
        func_800AFCA8(p->handle0, id);
    }
    p->done8 = 1;
}
