#include "common.h"
typedef short s16;
typedef unsigned short u16;
typedef struct { s16 delta; s16 index; void (*fn)(void); } VEntry;
typedef struct { s32 unk0; VEntry *vtbl; } List;
typedef struct { char pad0[0x24]; VEntry *vtbl; } Actor;
typedef struct { char pad0[0xB0]; List items; } Self;
extern Actor *D_801476B8;
extern char D_801C9E90[];
void *func_800EB9FC(Actor *);
char *func_80048480(u16);
void func_800498E4(s32, ...);
char *func_800D055C(List *);
char *func_80083C90(char *, char *);
s32 func_800CD4C4(void *, void *);
s32 func_800CD2BC(List *, void *);
s32 func_800CD538(void *, void *);
s32 func_80049CB4(s32, ...);
/* g++ 2.x vtable call: entry n holds {this delta, index, function} */
#define VFN(obj, n) ((obj)->vtbl[n].fn)
#define VTHIS(obj, n) ((char *)(obj) + (obj)->vtbl[n].delta)
s32 func_800DE230(Self *self) {
    s32 count;
    u32 i;
    void *inv;
    char *name;
    void *item;
    s32 skip;
    List *list;
    List *it;
    if (func_800EB9FC(D_801476B8) != 0) {
        func_800498E4(0xB6, func_80048480(0x46A));
        return 0;
    }
    count = 0;
    i = count;
    inv = ((void *(*)(void *))VFN(D_801476B8, 19))(VTHIS(D_801476B8, 19));
    list = &self->items;
    name = func_80083C90(D_801C9E90, func_800D055C(list));
    it = list;
    while (1) {
        if (!(i < ((s32 (*)(void *))VFN(it, 4))(VTHIS(it, 4)))) {
            break;
        }
        item = ((void *(*)(void *, u32))VFN(it, 7))(VTHIS(it, 7), i);
        skip = 0;
        if (((s32 (*)(void *, void *, s32))VFN(it, 12))(VTHIS(it, 12), item, skip) == 0 || func_800CD4C4(inv, item) == 0) {
            skip = 1;
        }
        if (skip) {
            i++;
        } else {
            func_800CD2BC(&self->items, item);
            func_800CD538(inv, item);
            count++;
        }
    }
    if (count > 0) {
        func_80049CB4(0x3C, D_801476B8);
        func_800498E4(0x96, name, count);
    } else {
        func_800498E4(0x97, name, count);
    }
    return 0;
}
