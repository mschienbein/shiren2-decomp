#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { s32 w[12]; } Item;
typedef struct { s16 delta; s16 pad; Item *(*fn)(void *, u32); } GetEntry;
typedef struct { s16 delta; s16 pad; void (*fn)(void *, s32, Item *); } SetEntry;
typedef struct { u8 pad[0x38]; GetEntry get; SetEntry set; } VTable;
typedef struct { s32 unk0; VTable *vt; } Inventory;
void func_800AE648(Item *);
void func_800D09D8(Inventory *inv, s32 k1, s32 k2) {
    Item *a;
    Item *b;
    Item tmp;
    a = inv->vt->get.fn((u8 *)inv + inv->vt->get.delta, k1);
    b = inv->vt->get.fn((u8 *)inv + inv->vt->get.delta, k2);
    if (a == 0) {
        inv->vt->set.fn((u8 *)inv + inv->vt->set.delta, k1, b);
        inv->vt->set.fn((u8 *)inv + inv->vt->set.delta, k2, 0);
        a = inv->vt->get.fn((u8 *)inv + inv->vt->get.delta, k1);
        if (a != 0) func_800AE648(a);
    } else if (b == 0) {
        inv->vt->set.fn((u8 *)inv + inv->vt->set.delta, k2, a);
        inv->vt->set.fn((u8 *)inv + inv->vt->set.delta, k1, 0);
        b = inv->vt->get.fn((u8 *)inv + inv->vt->get.delta, k2);
        if (b != 0) func_800AE648(b);
    } else {
        tmp = *a;
        *a = *b;
        *b = tmp;
        func_800AE648(a);
        func_800AE648(b);
    }
}
