#include "common.h"

typedef struct { void *container; void *item; } Item;
/* The complete embedded list occupies +0xA8 through +0xBB. */
typedef struct { char pad0[8]; Item *items; s32 capacity; s32 count; } List;
/* Whole 0x40C-byte menu, ending before D_801408EC. */
typedef struct { char pad[0x2CC]; s32 x2CC; char pad2[0x1C]; s32 (*x2EC)(void *); char pad2F0[0x11C]; } Loader;
typedef struct { char pad[0xA8]; List listA8; s32 xBC; char padC0[0xC]; void *xCC; s32 xD0; } S;
extern Loader D_801404E0;
extern s32 D_8013906C[2];
s32 func_8009A364(void *item);
s32 func_800CE46C(void *list, s32 (*pred)(void *));
void func_80097B90(void *obj, void *owner, void *holder, s32 mode, void *desc, s32 flag);
s32 func_800957C0(void *obj, void *out, s32 a2, void *a3, s32 a4);
s32 func_8009A038(Loader *l);
Item func_8009A054(void *obj, s32 index);
void func_800D05A4(List *list, Item *it);
s32 func_800D8514(S *p) {
    s32 head[2];
    Item item;
    s32 failed;
    s32 n, i;
    p->xD0 = 0;
    failed = func_800CE46C(p->xCC, func_8009A364) != 1;
    if (failed) return -3;
    func_80097B90(&D_801404E0, p->xCC, 0, 0x1000, D_8013906C, 0);
    D_801404E0.x2EC = func_8009A364;
    D_801404E0.x2CC = 1;
    failed = func_800957C0(&D_801404E0, &head, 1, 0, 0) != 1;
    if (failed) return -1;
    n = func_8009A038(&D_801404E0);
    i = 0;
    while (1) {
        if (!(i < n)) break;
        item = func_8009A054(&D_801404E0, i);
        func_800D05A4(&p->listA8, &item);
        i++;
    }
    p->xBC = 0;
    return 1;
}
