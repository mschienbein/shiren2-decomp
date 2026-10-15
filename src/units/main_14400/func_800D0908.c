#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
/* Item container: pool +0, +4 word, modified flag +8. */
typedef struct { ItemPool *handle0; s32 x4; s32 done8; } S;
void func_800AE648(Item *b);
void func_800D0908(S *p, s32 arg, Item *src) {
    u8 id = (u8)func_800D07D8(&p->handle0, arg);
    Item *dst = func_800AFD78(p->handle0, id);
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
